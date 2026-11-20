#include "match.h"
#include "matchlist.h"
#include "team.h"
#include "teamlist.h"
#include <stdio.h>

static int utf8_continuation_bytes(const char *s) {
    int n = 0;
    for (const unsigned char *p = (const unsigned char *)s; *p; p++)
        if ((*p & 0xC0) == 0x80)
            n++;
    return n;
}

int main(void) {
    FILE *f = fopen("times.csv", "rt");
    if (f == NULL)
        return 1;
    
    fscanf(f, "%*[^\n]\n");
    
    TeamList *tl = tl_create();
    
    int id;
    char name[50];
    while (fscanf(f, "%d,%49[^\r\n]", &id, name) == 2) {
        Team *t = team_create(id, name);
        if (t == NULL) {
            tl_free(tl);
            fclose(f);
            return 1;
        }
        tl_append(tl, t);
    }
    fclose(f);

    FILE *fp = fopen("partidas_completo.csv", "rt");
    if (fp == NULL) {
        tl_free(tl);
        return 1;
    }
    
    fscanf(fp, "%*[^\n]\n");
    
    MatchList *ml = ml_create();
    
    int idMatch;
    int t1;
    int t2;
    int golsT1;
    int golsT2;
    while (fscanf(fp, "%d,%d,%d,%d,%d", &idMatch, &t1, &t2, &golsT1, &golsT2) == 5) {
        Match *m = match_create(tl, idMatch, t1, t2, golsT1, golsT2);
        ml_append(ml, m);
    }
    fclose(fp);

    for (int i = 0; i < ml_size(ml); i++) {
        match_apply_stats(ml_get(ml, i));
    }

    TeamList *l = tl_create();

    for (int i = 0; i < tl_size(tl); i++) {
        Team *t = tl_get(tl, i);
        list_append_ordered(l, t);
    }

    FILE *classification = fopen("classificacao.csv", "wt");
    if (classification != NULL) {
        fprintf(classification, "Pos,ID,Time,V,E,D,GM,GS,S,PG\n");
        for (int i = 0; i < tl_size(l); i++) {
            Team *t = tl_get(l, i);
            fprintf(classification, "%d,%d,%s,%d,%d,%d,%d,%d,%d,%d\n",
                    i + 1, team_id(t), team_name(t),
                    team_wins(t), team_draws(t), team_losses(t),
                    team_goals_scored(t), team_goals_conceded(t),
                    team_goal_difference(t), team_points(t));
        }
        fclose(classification);
    }
    
    printf("POS  ID  Time            V   E   D   GM  GS  S   PG\n");
    for (int i = 0; i < tl_size(l); i++) {
        Team *t = tl_get(l, i);
        printf("%-4d %-3d %-15s", i + 1, team_id(t), team_name(t));
        for (int j = 0; j < utf8_continuation_bytes(team_name(t)); j++)
            putchar(' ');
        printf(" %-3d %-3d %-3d %-3d %-3d %-3d %-3d\n",
                team_wins(t), team_draws(t), team_losses(t),
                team_goals_scored(t), team_goals_conceded(t),
                team_goal_difference(t), team_points(t));
    }
    
    printf("\n===== Resumo do campeonato =====\n");

    if(ml_size(ml) > 0) {
        Team *champion = tl_get(l, 0);
        int points = team_points(champion);
        Team *bestAttackTeam = bestAttack(l);
        Team *bestDefenseTeam = bestDefense(l);
        Match *BiggestVictoryMatch = BiggestVictory(ml);
    
        printf("Partidas disputadas: %d de 45\n", ml_size(ml));
        if (ml_size(ml) == 45)
            printf("Campeão: %s (%d pontos)\n", team_name(champion), points);
        else
            printf("Líder: %s (%d pontos)\n", team_name(champion), points);
        printf("Melhor ataque: %s (%d gols marcados)\n", team_name(bestAttackTeam), team_goals_scored(bestAttackTeam));
        printf("Melhor defesa: %s (%d gols sofridos)\n", team_name(bestDefenseTeam), team_goals_conceded(bestDefenseTeam));
        printf("Maior goleada: %s %d x %d %s\n", team_name(match_t1(BiggestVictoryMatch)), match_golsT1(BiggestVictoryMatch), match_golsT2(BiggestVictoryMatch), team_name(match_t2(BiggestVictoryMatch)));
    } else {
        printf("Partidas disputadas: 0 de 45\n");
        printf("Nenhuma partida foi disputada.\n");
    }
    

    ml_free(ml);
    tl_free_nodes(l);
    tl_free(tl);
    return 0;
    }