#ifndef TEAMLIST_H
#define TEAMLIST_H

#include "team.h"

typedef struct teamlist TeamList;

TeamList *tl_create(void);

void tl_append(TeamList *tl, Team *t);

void list_append_ordered(TeamList *l, Team *t);

int tl_size(TeamList *tl);

Team *tl_get(TeamList *tl, int i);

Team *bestAttack(TeamList *tl);

Team *bestDefense(TeamList *tl);

void tl_free(TeamList *tl);

void tl_free_nodes(TeamList *tl);
#endif
