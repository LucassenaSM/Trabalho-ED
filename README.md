# Fechando o Campeonato

O projeto implementa o fechamento de um campeonato em execução em lote. O programa carrega os times de `times.csv`, apura as partidas informadas no arquivo de partidas, aplica os critérios de classificação, exibe a tabela e o resumo no terminal e gera o arquivo `classificacao.csv`.

## 1. Cabeçalho e Identificação

**Instituição:** IFES - Campus Serra  
**Curso:** Bacharelado em Sistemas de Informação  
**Disciplina:** Estrutura de Dados / Programação  
**Professor:** Thiago M. Paixão  
**Nome completo:** [PREENCHER]  
**Matrícula:** [PREENCHER]

## 2. Pré-requisitos, Compilação e Execução

O código foi preparado para C11 no ambiente Linux Ubuntu 22.04, usando GCC e Make.

Pré-requisitos:

- `gcc`
- `make`

Compile o projeto:

```sh
make compile
```

Compile e execute:

```sh
make
```

Também é possível executar diretamente o binário gerado:

```sh
./campeonato
```

Os arquivos objeto e o executável podem ser removidos com:

```sh
make clean
```

O `Makefile` usa `gcc` com `-Wall -Wextra -std=c11 -pedantic`, gera os objetos `main.o`, `team.o`, `teamlist.o`, `match.o` e `matchlist.o` e cria o executável `campeonato`.

Não existe menu ou entrada interativa. O programa lê `times.csv` e `partidas_completo.csv` a partir do diretório atual.

Para testar os três cenários fornecidos, faça uma cópia do cenário desejado sobre `partidas_completo.csv` antes de executar:

```sh
# Preserve o cenário completo antes das substituições
cp partidas_completo.csv partidas_completo_backup.csv

# Campeonato vazio
cp partidas_vazio.csv partidas_completo.csv
./campeonato

# Campeonato parcial
cp partidas_parcial.csv partidas_completo.csv
./campeonato

# Campeonato completo
cp partidas_completo_backup.csv partidas_completo.csv
./campeonato
```

Depois dos testes, o arquivo temporário `partidas_completo_backup.csv` pode ser removido.

No código atual, o arquivo efetivamente aberto para partidas é `partidas_completo.csv`; não existe leitura de `partidas.csv`.

## 3. Estrutura do Repositório

```text
.
|-- Makefile
|-- README.md
|-- main.c
|-- team.c
|-- team.h
|-- teamlist.c
|-- teamlist.h
|-- match.c
|-- match.h
|-- matchlist.c
|-- matchlist.h
|-- times.csv
|-- partidas_vazio.csv
|-- partidas_parcial.csv
|-- partidas_completo.csv
`-- classificacao.csv
```

- `main.c`: gerencia a carga dos CSVs, cria as listas, aplica os resultados das partidas, monta a classificação, mostra o relatório e escreve `classificacao.csv`.
- `team.c` e `team.h`: implementam o TAD opaco `Team`, incluindo identificação, nome e estatísticas do time.
- `teamlist.c` e `teamlist.h`: implementam o TAD opaco `TeamList`, uma lista duplamente encadeada de times.
- `match.c` e `match.h`: implementam o TAD opaco `Match`, representando uma partida e seus gols.
- `matchlist.c` e `matchlist.h`: implementam o TAD opaco `MatchList`, uma lista encadeada de partidas.
- `Makefile`: contém os comandos de compilação, execução e limpeza.
- `times.csv`: contém o cabeçalho `ID,Nome` e os times participantes.
- `partidas_vazio.csv`, `partidas_parcial.csv` e `partidas_completo.csv`: contém cada cenário de partidas fornecido ao projeto.
- `README.md`: documentação do projeto.
- `classificacao.csv`: arquivo gerado pelo programa com a classificação final ou parcial.

## 4. Tipos Abstratos de Dados (TADs) e Principais Funções

As quatro estruturas principais são opacas: `Team`, `TeamList`, `Match` e `MatchList` são declaradas nos arquivos `.h` como `typedef struct ...`, enquanto seus campos são definidos somente nos respectivos arquivos `.c`.

### `Team`

Responsável pelos dados e estatísticas de um time. Armazena `id`, `name`, `v`, `e`, `d`, `gm` e `gs`; saldo e pontos são calculados sob demanda e não são armazenados.

| Função pública | Responsabilidade |
|---|---|
| `team_create(id, name)` | Cria um time com os contadores estatísticos zerados. |
| `team_id(t)` | Retorna o identificador do time. |
| `team_name(t)` | Retorna o nome armazenado pelo time. |
| `team_wins(t)` | Retorna o número de vitórias. |
| `team_draws(t)` | Retorna o número de empates. |
| `team_losses(t)` | Retorna o número de derrotas. |
| `team_goals_scored(t)` | Retorna os gols marcados. |
| `team_goals_conceded(t)` | Retorna os gols sofridos. |
| `team_goal_difference(t)` | Calcula $GM - GS$. |
| `team_points(t)` | Calcula $3V + E$. |
| `team_update_stats(t, golsT1, golsT2)` | Atualiza vitória, empate, derrota, gols marcados e gols sofridos de um time. |
| `team_free(t)` | Libera o nome e o objeto do time. |

### `TeamList`

Responsável pela coleção principal de times e pela lista usada na classificação ordenada. A implementação usa nós com ponteiros `team`, `next` e `prev`.

| Função pública | Responsabilidade |
|---|---|
| `tl_create()` | Cria uma lista vazia. |
| `tl_append(tl, t)` | Insere um time ao final da lista principal. |
| `list_append_ordered(l, t)` | Insere um time na posição correspondente aos cinco critérios de classificação. |
| `tl_size(tl)` | Retorna a quantidade de nós da lista. |
| `tl_get(tl, i)` | Retorna o ponteiro do time na posição informada. |
| `bestAttack(tl)` | Seleciona o time com maior número de gols marcados; como a entrada já está ordenada por classificação, empates preservam essa melhor posição. |
| `bestDefense(tl)` | Seleciona o time com menor número de gols sofridos; como a entrada já está ordenada por classificação, empates preservam essa melhor posição. |
| `tl_free(tl)` | Libera os nós e também os objetos `Team`. |
| `tl_free_nodes(tl)` | Libera apenas os nós da lista ordenada, sem liberar os objetos `Team` compartilhados. |

### `Match`

Responsável por representar uma partida com os dois times e os gols de cada lado.

| Função pública | Responsabilidade |
|---|---|
| `match_create(tl, id, t1, t2, golsT1, golsT2)` | Localiza os dois times pelos IDs e cria a partida. |
| `match_id(m)` | Retorna o identificador da partida. |
| `match_t1(m)` | Retorna o ponteiro do time 1. |
| `match_t2(m)` | Retorna o ponteiro do time 2. |
| `match_golsT1(m)` | Retorna os gols do time 1. |
| `match_golsT2(m)` | Retorna os gols do time 2. |
| `match_apply_stats(m)` | Aplica o resultado da partida aos dois times participantes. |
| `match_free(m)` | Libera o objeto da partida, mas não libera os times associados a ela. |

### `MatchList`

Responsável por manter todas as partidas em uma lista encadeada na ordem em que foram lidas.

| Função pública | Responsabilidade |
|---|---|
| `ml_create()` | Cria uma lista vazia. |
| `ml_append(ml, m)` | Insere uma partida ao final da lista. |
| `ml_size(ml)` | Retorna a quantidade de partidas. |
| `ml_get(ml, i)` | Retorna a partida da posição informada. |
| `BiggestVictory(ml)` | Percorre as partidas e retorna a primeira que mantém a maior diferença de gols. |
| `ml_free(ml)` | Libera todas as partidas e os nós da lista. |

## 5. Principais Decisões de Implementação

### Estruturas de Dados

`TeamList` usa uma lista duplamente encadeada, com ponteiros `prev` e `next`, enquanto `MatchList` usa uma lista encadeada com nós que armazenam `Match*`. Os nós não possuem vetores dinâmicos nem arquivos auxiliares de configuração.

### Cálculo Dinâmico de Estatísticas

A struct `Team` não armazena saldo de gols ou pontos ganhos. O saldo é fornecido pela função `team_goal_difference(t)`, que calcula `GM - GS`; os pontos são fornecidos por `team_points(t)`, que calcula `3 * V + E`. Vitória, empate, derrota, gols marcados e gols sofridos são acumulados em `Team` por `team_update_stats()`.

Em uma partida, `match_apply_stats()` atualiza primeiro o time 1 e, depois, o time 2 com os gols invertidos. Isso garante que gols marcados e sofridos sejam atualizados nos dois lados.

### Ordenação e Gerenciamento de Memória

A classificação é construída com `list_append_ordered()` em uma segunda `TeamList`. Essa lista recebe os mesmos ponteiros `Team*` usados pela lista original, e não cria cópias dos times.

Os cinco critérios são aplicados nesta ordem:

1. Pontos ganhos: $3V + E$, maior primeiro.
2. Número de vitórias (`V`), maior primeiro.
3. Saldo de gols (`GM - GS`), maior primeiro.
4. Gols marcados (`GM`), maior primeiro.
5. Menor `ID` do time.

No encerramento, `tl_free_nodes(l)` libera somente os nós da lista ordenada. Em seguida, `tl_free(tl)` percorre a lista original e libera cada `Team` uma única vez, junto com seus nós. Nenhum objeto `Team` é compartilhado para a segunda liberação, portanto não ocorre double free.

### Alinhamento UTF-8 no Terminal

O nome mostrado na tabela usa `%-15s`. Como `õ` em `ESCorpiões` ocupa mais de um byte em UTF-8, a largura de bytes não corresponde à largura visual. A função `utf8_continuation_bytes()` conta os bytes de continuação da string e adiciona espaços extras antes das estatísticas, compensando o tamanho adicional dos caracteres multibyte.

### Tratamento dos Cenários de Campeonato

Com 0 partidas, todos os times possuem estatísticas zeradas e a classificação é organizada pelo menor ID. O terminal mostra `Partidas disputadas: 0 de 45` e `Nenhuma partida foi disputada.`; não são exibidos líder, ataque, defesa ou goleada.

Com menos de 45 partidas, o resumo usa `Líder` para o primeiro colocado.

Com exatamente 45 partidas, o resumo usa `Campeão` para o primeiro colocado.

No resumo:

- Melhor ataque recebe o time da lista já ordenada que tiver o maior `GM`.
- Melhor defesa recebe o time da lista já ordenada que tiver o menor `GS`.
- Empates de ataque e defesa são resolvidos pela melhor classificação.
- Maior goleada é a partida de maior diferença absoluta de gols.
- Empate na diferença mantém a primeira partida encontrada no arquivo.

## 6. Exemplo de Saída

Formato da tabela exibido no terminal:

```text
POS  ID  Time            V   E   D   GM  GS  S   PG
1    6   NETunos         5   2   2   21  9   12  17
2    7   LOOPardos       5   1   3   14  12  2   16
3    4   PYthons         4   3   2   13  9   4   15
4    1   ESCorpiões      4   3   2   14  19  -5  15

===== Resumo do campeonato =====
Partidas disputadas: 45 de 45
Campeão: NETunos (17 pontos)
Melhor ataque: NETunos (21 gols marcados)
Melhor defesa: NETunos (9 gols sofridos)
Maior goleada: ESCorpiões 0 x 5 RUSTicos
```

Formato do arquivo `classificacao.csv` gerado pelo programa:

```csv
Pos,ID,Time,V,E,D,GM,GS,S,PG
1,6,NETunos,5,2,2,21,9,12,17
2,7,LOOPardos,5,1,3,14,12,2,16
3,4,PYthons,4,3,2,13,9,4,15
4,1,ESCorpiões,4,3,2,14,19,-5,15
```
