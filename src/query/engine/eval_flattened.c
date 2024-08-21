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
