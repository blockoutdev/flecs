/**
 * @file query/engine/eval_flattened.c
 * @brief Flattened hierarchy iteration.
 */

#include "../../private_api.h"

bool flecs_query_flat(
    const ecs_query_op_t *op,
    bool redo,
    const ecs_query_run_ctx_t *ctx)
{
    ecs_query_and_flat_ctx_t *op_ctx = flecs_op_ctx(ctx, and_flat);

    if (!redo) {
        /* Initialize context for first result */

        /* Get component id to match. This will be a pair that looks like 
         * (ChildOf, parent) or (ChildOf, *). */
        op_ctx->flatten_id = flecs_query_op_get_id(op, ctx);

        /* Flattened iteration only applies to pairs that have a relationship
         * with the CanFlatten trait, so id must be a pair. */
        ecs_assert(ECS_IS_PAIR(op_ctx->flatten_id), ECS_INTERNAL_ERROR, NULL);

        /* Get first and second elements of pair */
        ecs_entity_t first = ECS_PAIR_FIRST(op_ctx->flatten_id);
        ecs_entity_t second = ECS_PAIR_SECOND(op_ctx->flatten_id);

        if (second != EcsWildcard) {
            /* We're matching a (ChildOf, parent) pair. */
            second = flecs_entities_get_alive(ctx->world, second);
            if (!second) {
                /* This is a query for root entities, and root entities can't
                 * be flattened. (R, 0) pairs are only valid if R is ChildOf. */
                ecs_assert(first == EcsChildOf, ECS_INTERNAL_ERROR, NULL);
                return false;
            }

            /* Get the (Children, R) component from the parent. If the matched
             * relationship is ChildOf, this will fetch (Children, ChildOf). */
            const EcsChildren *children = ecs_get_pair(
                ctx->world, second, EcsChildren, first);
            if (!children) {
                return false;
            }

            /* Prepare the query context with the children array, number of 
             * children and index of the currently iterated child. */
            op_ctx->children = ecs_vec_first_t(
                &children->children, ecs_entity_t);
            op_ctx->children_count = ecs_vec_count(&children->children);
            op_ctx->cur_child = -1;
        }
    }

    /* Move to the next child in the children array. */
    ecs_assert(op_ctx->cur_child < op_ctx->children_count, 
        ECS_INTERNAL_ERROR, NULL);
    int32_t cur = ++ op_ctx->cur_child;
    if (op_ctx->cur_child == op_ctx->children_count) {
        return false;
    }

    /* Assign the child to the source of the operation. This will initialize the
     * source variable (typically $this) with the entity, which allows 
     * subsequent operations that use the same variable to use the entity. */
    flecs_query_set_src(op, op_ctx->children[cur], ctx);
    
    return true;
}

bool flecs_query_ids_flat(
    const ecs_query_op_t *op,
    const ecs_query_run_ctx_t *ctx,
    ecs_id_t id)
{
    /* If id is not a pair, this check can't be for a flattened relationship. */
    if (ECS_IS_PAIR(id)) {
        ecs_id_t wc = ecs_pair(ECS_PAIR_FIRST(id), EcsWildcard);
        ecs_id_record_t *idr = flecs_id_record_get(ctx->world, wc);
        if (!idr) {
            /* Id record should exist for flattened */
            return false;
        }

        if (idr->flags & EcsIdCanFlatten) {
            ecs_entity_t first = ECS_PAIR_FIRST(id);
            ecs_entity_t second = ECS_PAIR_SECOND(id);

            /* The pair has a relationship that can be flattened. Check
             * if second element of the pair flattened children. */
            ecs_entity_t parent = flecs_entities_get_alive(ctx->world, second);
            ecs_assert(parent != 0, ECS_INTERNAL_ERROR, NULL);

            /* Check for (Children, R) component which stores flattened trees 
             * for relationship R. */
            if (ecs_owns_pair(ctx->world, parent, ecs_id(EcsChildren), first)) {
                /* Parent has flattened children */
                return true;
            }
        }
    }

    return false;
}
