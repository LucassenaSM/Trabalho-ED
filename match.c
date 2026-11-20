#include "match.h"
#include "team.h"
#include "teamlist.h"
#include <stddef.h>
#include <stdlib.h>

struct match {
    int id;
    Team *t1;
    Team *t2;
    int golsT1;
    int golsT2;
};

Match *match_create(TeamList *tl, int id, int t1, int t2, int golsT1, int golsT2) {
    if (tl == NULL)
        return NULL;

    Team *team1 = NULL;
    Team *team2 = NULL;
    int i;

    for (i = 0; i < tl_size(tl); i++) {
        Team *team = tl_get(tl, i);
        if (team_id(team) == t1)
            team1 = team;
        if (team_id(team) == t2)
            team2 = team;
    }

    if (team1 == NULL || team2 == NULL)
        return NULL;

    Match *new_match = malloc(sizeof(*new_match));
    if (new_match == NULL)
        return NULL;

    new_match->id = id;
    new_match->t1 = team1;
    new_match->t2 = team2;
    new_match->golsT1 = golsT1;
    new_match->golsT2 = golsT2;

    return new_match;
}

int match_id(Match *m) {
    return m != NULL ? m->id : 0;
}

Team *match_t1(Match *m) {
    return m != NULL ? m->t1 : NULL;
}

Team *match_t2(Match *m) {
    return m != NULL ? m->t2 : NULL;
}

int match_golsT1(Match *m) {
    return  m != NULL ? m->golsT1 : 0;
}

int match_golsT2(Match *m) {
    return  m != NULL ? m->golsT2 : 0;
}

void match_apply_stats(Match *m) {
    if (m == NULL)
        return;
    team_update_stats(m->t1, m->golsT1, m->golsT2);
    team_update_stats(m->t2, m->golsT2, m->golsT1);
}

void match_free(Match *m) {
    if (m == NULL)
        return;

    free(m);
}