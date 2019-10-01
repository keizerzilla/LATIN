#include "../include/pointset.h"

struct pointset *pointset_new()
{
	return NULL;
}

struct vector3 *pointset_insert_real(struct pointset **set,
                                     real x,
                                     real y,
                                     real z)
{
	struct pointset *new = malloc(sizeof(struct pointset));
	if (new == NULL)
		return NULL;
	
	new->point = vector3_new(x, y, z);
	if (new->point == NULL)
		return NULL;
	
	new->next = *set;
	new->prev = NULL;
	
	if (*set != NULL)
		(*set)->prev = new;
	
	*set = new;
	
	return new->point;
}

struct vector3 *pointset_insert_vector3(struct pointset **set,
                                        struct vector3 *v)
{
	return pointset_insert_real(set, v->x, v->y, v->z);
}

struct pointset *pointset_tail(struct pointset *set)
{
	struct pointset *temp = set;
	
	while (temp != NULL && temp->next != NULL)
		temp = temp->next;
	
	return temp;
}

void pointset_swap(struct vector3 **a, struct vector3 **b)
{
	struct vector3 *tmp = *a;
	
	*a = *b;
	*b = tmp;
}

struct pointset *pointset_partition(struct pointset *head,
                                    struct pointset *end,
                                    int axis)
{
	real x = end->point->coord[axis];
	
	struct pointset *i = head->prev;
	
	for (struct pointset *j = head; j != end; j = j->next) {
		if (j->point->coord[axis] <= x) {
			i = (i == NULL) ? head : i->next;
			
			pointset_swap(&(i->point), &(j->point));
		}
	}
	
	i = (i == NULL) ? head : i->next;
	pointset_swap(&(i->point), &(end->point));
	
	return i;
}

void pointset_recursive_sort(struct pointset *head,
                             struct pointset *end,
                             int axis)
{
	if (end != NULL && head != end && head != end->next) {
		struct pointset *p = pointset_partition(head, end, axis);
		
		pointset_recursive_sort(head, p->prev, axis);
		pointset_recursive_sort(p->next, end, axis);
	}
}

void pointset_sort(struct pointset *set, int axis)
{
	struct pointset *end = pointset_tail(set);
	pointset_recursive_sort(set, end, axis);
}

void pointset_debug(struct pointset *set, FILE *output)
{
	for (struct pointset *s = set; s != NULL; s = s->next)
		vector3_debug(s->point, output);
}

// TRANSFORMAR EM FOR
void pointset_debug_reverse(struct pointset *set, FILE *output)
{
	struct pointset *tail = pointset_tail(set);
	
	for (struct pointset *s = tail; s != NULL; s = s->next)
		vector3_debug(s->point, output);
}

void pointset_free(struct pointset **set)
{
	if (*set == NULL)
		return;
	
	vector3_free(&(*set)->point);
	pointset_free(&(*set)->next);
	free(*set);
}

