#include <query.h>

static ecs_query_cache_kind_t cache_kind = EcsQueryCacheDefault;

void Flatten_setup(void) {
    const char *cache_param = test_param("cache_kind");
    if (cache_param) {
        if (!strcmp(cache_param, "default")) {
            // already set to default
        } else if (!strcmp(cache_param, "auto")) {
            cache_kind = EcsQueryCacheAuto;
        } else {
            printf("unexpected value for cache_param '%s'\n", cache_param);
        }
    }
}

void Flatten_this_childof_parent(void) {
    ecs_world_t *world = ecs_mini();

    ecs_entity_t p = ecs_new_w_id(world, EcsPrefab);
    ecs_entity_t c_1 = ecs_new_w_pair(world, EcsChildOf, p);
    ecs_entity_t c_2 = ecs_new_w_pair(world, EcsChildOf, p);
    ecs_entity_t c_3 = ecs_new_w_pair(world, EcsChildOf, p);

    ecs_entity_t i = ecs_new_w_pair(world, EcsIsA, p);
    const EcsChildren *children = ecs_get_pair(
        world, i, EcsChildren, EcsChildOf);
    test_assert(children != NULL);

    ecs_entity_t *child_ids = ecs_vec_first(&children->children);
    test_assert(child_ids != NULL);
    test_int(ecs_vec_count(&children->children), 3);
    test_assert(ecs_has_pair(world, child_ids[0], EcsIsA, c_1));
    test_assert(ecs_has_pair(world, child_ids[1], EcsIsA, c_2));
    test_assert(ecs_has_pair(world, child_ids[2], EcsIsA, c_3));

    ecs_query_t *q = ecs_query(world, {
        .terms = {{ ecs_childof(i) }},
        .cache_kind = cache_kind
    });

    test_assert(q != NULL);

    ecs_iter_t it = ecs_query_iter(world, q);
    test_bool(true, ecs_query_next(&it));
    test_int(1, it.count);
    test_uint(it.entities[0], child_ids[0]);
    test_assert(it.table == ecs_get_table(world, child_ids[0]));
    test_uint(ecs_pair(EcsChildOf, i), ecs_field_id(&it, 0));
    test_bool(true, ecs_field_is_set(&it, 0));

    test_bool(true, ecs_query_next(&it));
    test_int(1, it.count);
    test_uint(it.entities[0], child_ids[1]);
    test_assert(it.table == ecs_get_table(world, child_ids[1]));
    test_uint(ecs_pair(EcsChildOf, i), ecs_field_id(&it, 0));
    test_bool(true, ecs_field_is_set(&it, 0));

    test_bool(true, ecs_query_next(&it));
    test_int(1, it.count);
    test_uint(it.entities[0], child_ids[2]);
    test_assert(it.table == ecs_get_table(world, child_ids[2]));
    test_uint(ecs_pair(EcsChildOf, i), ecs_field_id(&it, 0));
    test_bool(true, ecs_field_is_set(&it, 0));

    test_bool(false, ecs_query_next(&it));

    ecs_query_fini(q);

    ecs_fini(world);
}

void Flatten_this_childof_parent_no_children(void) {
    ecs_world_t *world = ecs_mini();

    ecs_entity_t p = ecs_new_w_id(world, EcsPrefab);

    ecs_entity_t i = ecs_new_w_pair(world, EcsIsA, p);
    const EcsChildren *children = ecs_get_pair(
        world, i, EcsChildren, EcsChildOf);
    test_assert(children == NULL);

    ecs_query_t *q = ecs_query(world, {
        .terms = {{ ecs_childof(i) }},
        .cache_kind = cache_kind
    });

    test_assert(q != NULL);

    ecs_iter_t it = ecs_query_iter(world, q);
    test_bool(false, ecs_query_next(&it));

    ecs_query_fini(q);

    ecs_fini(world);
}

void Flatten_this_childof_parent_only_regular(void) {
    ecs_world_t *world = ecs_mini();

    ecs_entity_t i = ecs_new(world);
    ecs_entity_t c_1 = ecs_new_w_pair(world, EcsChildOf, i);
    ecs_entity_t c_2 = ecs_new_w_pair(world, EcsChildOf, i);
    ecs_entity_t c_3 = ecs_new_w_pair(world, EcsChildOf, i);

    ecs_query_t *q = ecs_query(world, {
        .terms = {{ ecs_childof(i) }},
        .cache_kind = cache_kind
    });

    test_assert(q != NULL);

    ecs_iter_t it = ecs_query_iter(world, q);
    test_bool(true, ecs_query_next(&it));
    test_int(3, it.count);
    test_uint(it.entities[0], c_1);
    test_uint(it.entities[1], c_2);
    test_uint(it.entities[2], c_3);
    test_assert(it.table == ecs_get_table(world, c_1));
    test_assert(it.table == ecs_get_table(world, c_2));
    test_assert(it.table == ecs_get_table(world, c_3));
    test_uint(ecs_pair(EcsChildOf, i), ecs_field_id(&it, 0));
    test_bool(true, ecs_field_is_set(&it, 0));
    test_bool(false, ecs_query_next(&it));

    ecs_query_fini(q);

    ecs_fini(world);
}

void Flatten_this_childof_parent_mixed(void) {
    ecs_world_t *world = ecs_mini();

    ecs_entity_t p = ecs_new_w_id(world, EcsPrefab);
    ecs_entity_t c_1 = ecs_new_w_pair(world, EcsChildOf, p);
    ecs_entity_t c_2 = ecs_new_w_pair(world, EcsChildOf, p);
    ecs_entity_t c_3 = ecs_new_w_pair(world, EcsChildOf, p);

    ecs_entity_t i = ecs_new_w_pair(world, EcsIsA, p);
    const EcsChildren *children = ecs_get_pair(
        world, i, EcsChildren, EcsChildOf);
    test_assert(children != NULL);

    ecs_entity_t *child_ids = ecs_vec_first(&children->children);
    test_assert(child_ids != NULL);
    test_int(ecs_vec_count(&children->children), 3);
    test_assert(ecs_has_pair(world, child_ids[0], EcsIsA, c_1));
    test_assert(ecs_has_pair(world, child_ids[1], EcsIsA, c_2));
    test_assert(ecs_has_pair(world, child_ids[2], EcsIsA, c_3));

    ecs_entity_t ic_1 = ecs_new_w_pair(world, EcsChildOf, i);
    ecs_entity_t ic_2 = ecs_new_w_pair(world, EcsChildOf, i);
    ecs_entity_t ic_3 = ecs_new_w_pair(world, EcsChildOf, i);

    ecs_query_t *q = ecs_query(world, {
        .terms = {{ ecs_childof(i) }},
        .cache_kind = cache_kind
    });

    test_assert(q != NULL);

    ecs_iter_t it = ecs_query_iter(world, q);
    test_bool(true, ecs_query_next(&it));
    test_int(3, it.count);
    test_uint(it.entities[0], ic_1);
    test_uint(it.entities[1], ic_2);
    test_uint(it.entities[2], ic_3);
    test_assert(it.table == ecs_get_table(world, ic_1));
    test_assert(it.table == ecs_get_table(world, ic_2));
    test_assert(it.table == ecs_get_table(world, ic_3));
    test_uint(ecs_pair(EcsChildOf, i), ecs_field_id(&it, 0));
    test_bool(true, ecs_field_is_set(&it, 0));

    test_bool(true, ecs_query_next(&it));
    test_int(1, it.count);
    test_uint(it.entities[0], child_ids[0]);
    test_assert(it.table == ecs_get_table(world, child_ids[0]));
    test_uint(ecs_pair(EcsChildOf, i), ecs_field_id(&it, 0));
    test_bool(true, ecs_field_is_set(&it, 0));

    test_bool(true, ecs_query_next(&it));
    test_int(1, it.count);
    test_uint(it.entities[0], child_ids[1]);
    test_assert(it.table == ecs_get_table(world, child_ids[1]));
    test_uint(ecs_pair(EcsChildOf, i), ecs_field_id(&it, 0));
    test_bool(true, ecs_field_is_set(&it, 0));

    test_bool(true, ecs_query_next(&it));
    test_int(1, it.count);
    test_uint(it.entities[0], child_ids[2]);
    test_assert(it.table == ecs_get_table(world, child_ids[2]));
    test_uint(ecs_pair(EcsChildOf, i), ecs_field_id(&it, 0));
    test_bool(true, ecs_field_is_set(&it, 0));

    test_bool(false, ecs_query_next(&it));

    ecs_query_fini(q);

    ecs_fini(world);
}

void Flatten_this_childof_parent_w_tag(void) {
    ecs_world_t *world = ecs_mini();

    ECS_TAG(world, Foo);

    ecs_entity_t p = ecs_new_w_id(world, EcsPrefab);
    ecs_entity_t c_1 = ecs_new_w_pair(world, EcsChildOf, p);
    ecs_entity_t c_2 = ecs_new_w_pair(world, EcsChildOf, p);
    ecs_entity_t c_3 = ecs_new_w_pair(world, EcsChildOf, p);
    ecs_entity_t c_4 = ecs_new_w_pair(world, EcsChildOf, p);

    ecs_add(world, c_2, Foo);
    ecs_add(world, c_4, Foo);

    ecs_entity_t i = ecs_new_w_pair(world, EcsIsA, p);
    const EcsChildren *children = ecs_get_pair(
        world, i, EcsChildren, EcsChildOf);
    test_assert(children != NULL);

    ecs_entity_t *child_ids = ecs_vec_first(&children->children);
    test_assert(child_ids != NULL);
    test_int(ecs_vec_count(&children->children), 4);
    test_assert(ecs_has_pair(world, child_ids[0], EcsIsA, c_1));
    test_assert(ecs_has_pair(world, child_ids[1], EcsIsA, c_3));
    test_assert(ecs_has_pair(world, child_ids[2], EcsIsA, c_2));
    test_assert(ecs_has_pair(world, child_ids[3], EcsIsA, c_4));

    test_assert(!ecs_has(world, child_ids[0], Foo));
    test_assert(!ecs_has(world, child_ids[1], Foo));
    test_assert(ecs_has(world, child_ids[2], Foo));
    test_assert(ecs_has(world, child_ids[3], Foo));

    ecs_query_t *q = ecs_query(world, {
        .terms = {{ ecs_childof(i) }, { Foo }},
        .cache_kind = cache_kind
    });

    test_assert(q != NULL);

    ecs_iter_t it = ecs_query_iter(world, q);
    test_bool(true, ecs_query_next(&it));
    test_int(1, it.count);
    test_uint(it.entities[0], child_ids[2]);
    test_assert(it.table == ecs_get_table(world, child_ids[2]));
    test_uint(ecs_pair(EcsChildOf, i), ecs_field_id(&it, 0));
    test_uint(Foo, ecs_field_id(&it, 1));
    test_bool(true, ecs_field_is_set(&it, 0));
    test_bool(true, ecs_field_is_set(&it, 1));

    test_bool(true, ecs_query_next(&it));
    test_int(1, it.count);
    test_uint(it.entities[0], child_ids[3]);
    test_assert(it.table == ecs_get_table(world, child_ids[3]));
    test_uint(ecs_pair(EcsChildOf, i), ecs_field_id(&it, 0));
    test_uint(Foo, ecs_field_id(&it, 1));
    test_bool(true, ecs_field_is_set(&it, 0));
    test_bool(true, ecs_field_is_set(&it, 1));

    test_bool(false, ecs_query_next(&it));

    ecs_query_fini(q);

    ecs_fini(world);
}

void Flatten_this_childof_parent_w_component(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);

    ecs_entity_t p = ecs_new_w_id(world, EcsPrefab);
    ecs_entity_t c_1 = ecs_new_w_pair(world, EcsChildOf, p);
    ecs_entity_t c_2 = ecs_new_w_pair(world, EcsChildOf, p);
    ecs_entity_t c_3 = ecs_new_w_pair(world, EcsChildOf, p);
    ecs_entity_t c_4 = ecs_new_w_pair(world, EcsChildOf, p);

    ecs_set(world, c_2, Position, {10, 20});
    ecs_set(world, c_4, Position, {30, 40});

    ecs_entity_t i = ecs_new_w_pair(world, EcsIsA, p);
    const EcsChildren *children = ecs_get_pair(
        world, i, EcsChildren, EcsChildOf);
    test_assert(children != NULL);

    ecs_entity_t *child_ids = ecs_vec_first(&children->children);
    test_assert(child_ids != NULL);
    test_int(ecs_vec_count(&children->children), 4);
    test_assert(ecs_has_pair(world, child_ids[0], EcsIsA, c_1));
    test_assert(ecs_has_pair(world, child_ids[1], EcsIsA, c_3));
    test_assert(ecs_has_pair(world, child_ids[2], EcsIsA, c_2));
    test_assert(ecs_has_pair(world, child_ids[3], EcsIsA, c_4));

    test_assert(!ecs_has(world, child_ids[0], Position));
    test_assert(!ecs_has(world, child_ids[1], Position));
    test_assert(ecs_has(world, child_ids[2], Position));
    test_assert(ecs_has(world, child_ids[3], Position));

    ecs_query_t *q = ecs_query(world, {
        .terms = {{ ecs_childof(i) }, { ecs_id(Position) }},
        .cache_kind = cache_kind
    });

    test_assert(q != NULL);

    ecs_iter_t it = ecs_query_iter(world, q);
    test_bool(true, ecs_query_next(&it));
    test_int(1, it.count);
    test_uint(it.entities[0], child_ids[2]);
    test_assert(it.table == ecs_get_table(world, child_ids[2]));
    test_uint(ecs_pair(EcsChildOf, i), ecs_field_id(&it, 0));
    test_uint(ecs_id(Position), ecs_field_id(&it, 1));
    test_bool(true, ecs_field_is_set(&it, 0));
    test_bool(true, ecs_field_is_set(&it, 1));
    {
        Position *p = ecs_field(&it, Position, 1);
        test_assert(p != NULL);
        test_int(p->x, 10);
        test_int(p->y, 20);
    }

    test_bool(true, ecs_query_next(&it));
    test_int(1, it.count);
    test_uint(it.entities[0], child_ids[3]);
    test_assert(it.table == ecs_get_table(world, child_ids[3]));
    test_uint(ecs_pair(EcsChildOf, i), ecs_field_id(&it, 0));
    test_uint(ecs_id(Position), ecs_field_id(&it, 1));
    test_bool(true, ecs_field_is_set(&it, 0));
    test_bool(true, ecs_field_is_set(&it, 1));
    {
        Position *p = ecs_field(&it, Position, 1);
        test_assert(p != NULL);
        test_int(p->x, 30);
        test_int(p->y, 40);
    }

    test_bool(false, ecs_query_next(&it));

    ecs_query_fini(q);

    ecs_fini(world);
}
