#ifndef MATCH_H
#define MATCH_H

#include "team.h"
#include "teamlist.h"
typedef struct match Match;

Match *match_create(TeamList *tl, int id, int t1, int t2, int golsT1, int golsT2);

int match_id(Match *m);

Team *match_t1(Match *m);

Team *match_t2(Match *m);

int match_golsT1(Match *m);

int match_golsT2(Match *m);

void match_apply_stats(Match *m);

void match_free(Match *m);

#endif