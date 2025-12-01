#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// ============================================================================
// Constantes Globais
// ============================================================================
#define MAX_TERRITORIOS 10
#define TAM_NOME        32
#define NUM_MISSOES     4

// Cor do exército do jogador
#define COR_JOGADOR     'V'   // V = Verde (exemplo)

// ============================================================================
// Estruturas
// ============================================================================
typedef struct {
    char nome[TAM_NOME];
    char corExercito;  // Ex: 'V' (jogador), 'R', 'A', etc.
    int  tropas;
} Territorio;

// ============================================================================
// Protótipos das Funções
// ============================================================================

// Setup / memória
Territorio* alocarMapa(int qtdTerritorios);
void        inicializarTerritorios(Territorio *mapa, int qtdTerritorios);
void        liberarMemoria(Territorio **mapa);

// Interface usuário
void exibirMenuPrincipal(void);
void exibirMapa(const Territorio *mapa, int qtdTerritorios);
void exibirMissao(int idMissao);

// Lógica de jogo
void faseDeAtaque(Territorio *mapa, int qtdTerritorios, char corJogador);
void simularAtaque(Territorio *origem, Territorio *destino, char corJogador);
int  sortearMissao(void);
int  verificarVitoria(const Territorio *mapa, int qtdTerritorios,
                      char corJogador, int idMissao);

// Utilitárias
void limparBufferEntrada(void);

// ============================================================================
// Função Principal
// ============================================================================
int main(void) {
    // 1. Configuração Inicial (Setup)
    setlocale(LC_ALL, "Portuguese"); // Pode variar conforme o SO (pt_BR.UTF-8, etc.)
    srand((unsigned)time(NULL));

    Territorio *mapa = alocarMapa(MAX_TERRITORIOS);
    if (mapa == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para o mapa.\n");
        return 1;
    }

    inicializarTerritorios(mapa, MAX_TERRITORIOS);

    char corJogador = COR_JOGADOR;
    int  idMissao   = sortearMissao();

    int opcao;
    int venceu = 0;

    // 2. Laço Principal do Jogo (Game Loop)
    do {
        printf("\n==================== GUERRA ESTRUTURADA ====================\n");
        printf("Cor do seu exercito: %c\n", corJogador);
        exibirMapa(mapa, MAX_TERRITORIOS);
        exibirMissao(idMissao);

        exibirMenuPrincipal();
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            limparBufferEntrada();
            printf("Entrada invalida.\n");
            continue;
        }
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                // Fase de ataque
                faseDeAtaque(mapa, MAX_TERRITORIOS, corJogador);
                break;

            case 2:
                // Verificar condicao de vitoria
                if (verificarVitoria(mapa, MAX_TERRITORIOS, corJogador, idMissao)) {
                    printf("\n*** PARABENS! VOCE CUMPRIU SUA MISSAO! ***\n");
                    venceu = 1;
                } else {
                    printf("\nAinda nao cumpriu sua missao. Continue lutando!\n");
                }
                break;

            case 0:
                printf("\nEncerrando o jogo...\n");
                break;

            default:
                printf("\nOpcao invalida.\n");
                break;
        }

        if (!venceu && opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while (opcao != 0 && !venceu);

    // 3. Limpeza
    liberarMemoria(&mapa);

    return 0;
}

// ============================================================================
// Implementação das Funções
// ============================================================================

// ---------------------------------------------------------------------
// alocarMapa()
// ---------------------------------------------------------------------
Territorio* alocarMapa(int qtdTerritorios) {
    // calloc zera a memoria
    Territorio *mapa = (Territorio*)calloc(qtdTerritorios, sizeof(Territorio));
    return mapa;
}

// ---------------------------------------------------------------------
// inicializarTerritorios()
// ---------------------------------------------------------------------
void inicializarTerritorios(Territorio *mapa, int qtdTerritorios) {
    // Exemplo simples de mundo com 10 territorios
    const char *nomes[MAX_TERRITORIOS] = {
        "Alfa", "Bravo", "Charlie", "Delta", "Echo",
        "Foxtrot", "Golf", "Hotel", "India", "Juliet"
    };

    const char cores[MAX_TERRITORIOS] = {
        'V', 'R', 'A', 'R', 'A',   // 0-4
        'V', 'R', 'A', 'V', 'R'    // 5-9
    };

    const int tropasIniciais[MAX_TERRITORIOS] = {
        3, 2, 2, 1, 1,
        3, 2, 1, 4, 2
    };

    for (int i = 0; i < qtdTerritorios; i++) {
        strncpy(mapa[i].nome, nomes[i], TAM_NOME - 1);
        mapa[i].nome[TAM_NOME - 1] = '\0';
        mapa[i].corExercito = cores[i];
        mapa[i].tropas      = tropasIniciais[i];
    }
}

// ---------------------------------------------------------------------
// liberarMemoria()
// ---------------------------------------------------------------------
void liberarMemoria(Territorio **mapa) {
    if (mapa != NULL && *mapa != NULL) {
        free(*mapa);
        *mapa = NULL;
    }
}

// ---------------------------------------------------------------------
// exibirMenuPrincipal()
// ---------------------------------------------------------------------
void exibirMenuPrincipal(void) {
    printf("\n-------------------- MENU PRINCIPAL --------------------\n");
    printf("1 - Fase de ataque\n");
    printf("2 - Verificar se cumpriu a missao\n");
    printf("0 - Sair\n");
}

// ---------------------------------------------------------------------
// exibirMapa()
// ---------------------------------------------------------------------
void exibirMapa(const Territorio *mapa, int qtdTerritorios) {
    printf("\n-------------------- MAPA --------------------\n");
    printf("%-3s %-15s %-8s %-6s\n", "#", "Territorio", "Cor", "Tropas");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < qtdTerritorios; i++) {
        printf("%-3d %-15s %-8c %-6d\n",
               i, mapa[i].nome, mapa[i].corExercito, mapa[i].tropas);
    }
}

// ---------------------------------------------------------------------
// exibirMissao()
// ---------------------------------------------------------------------
void exibirMissao(int idMissao) {
    printf("\n-------------------- MISSAO SECRETA --------------------\n");
    switch (idMissao) {
        case 0:
            printf("Missao 0: Controlar pelo menos 5 territorios quaisquer.\n");
            break;
        case 1:
            printf("Missao 1: Controlar todos os territorios do mapa.\n");
            break;
        case 2:
            printf("Missao 2: Destruir completamente o exercito Vermelho (R).\n");
            break;
        case 3:
            printf("Missao 3: Ter ao menos 3 territorios seus com 5 ou mais tropas.\n");
            break;
        default:
            printf("Missao desconhecida.\n");
            break;
    }
}

// ---------------------------------------------------------------------
// faseDeAtaque()
// ---------------------------------------------------------------------
void faseDeAtaque(Territorio *mapa, int qtdTerritorios, char corJogador) {
    int origem, destino;
    char continuar = 's';

    do {
        printf("\n-------------------- FASE DE ATAQUE --------------------\n");

        printf("Seus territorios (%c):\n", corJogador);
        for (int i = 0; i < qtdTerritorios; i++) {
            if (mapa[i].corExercito == corJogador) {
                printf("%2d - %-15s Tropas: %d\n",
                       i, mapa[i].nome, mapa[i].tropas);
            }
        }

        printf("\nDigite o indice do territorio de ORIGEM (-1 para voltar): ");
        if (scanf("%d", &origem) != 1) {
            limparBufferEntrada();
            printf("Entrada invalida.\n");
            return;
        }
        limparBufferEntrada();

        if (origem < 0) {
            // Sair da fase de ataque
            break;
        }

        if (origem < 0 || origem >= qtdTerritorios ||
            mapa[origem].corExercito != corJogador) {
            printf("Territorio de origem invalido.\n");
            continue;
        }

        if (mapa[origem].tropas < 2) {
            printf("Voce precisa de pelo menos 2 tropas para atacar.\n");
            continue;
        }

        printf("Digite o indice do territorio de DESTINO: ");
        if (scanf("%d", &destino) != 1) {
            limparBufferEntrada();
            printf("Entrada invalida.\n");
            return;
        }
        limparBufferEntrada();

        if (destino < 0 || destino >= qtdTerritorios || destino == origem) {
            printf("Territorio de destino invalido.\n");
            continue;
        }

        if (mapa[destino].corExercito == corJogador) {
            printf("Voce ja controla esse territorio.\n");
            continue;
        }

        // Para simplificar, nao checamos adjacencia: qualquer territorio pode atacar qualquer outro.
        simularAtaque(&mapa[origem], &mapa[destino], corJogador);

        printf("\nDeseja realizar outro ataque? (s/n): ");
        scanf(" %c", &continuar);
        limparBufferEntrada();

    } while (continuar == 's' || continuar == 'S');
}

// ---------------------------------------------------------------------
// simularAtaque()
// ---------------------------------------------------------------------
void simularAtaque(Territorio *origem, Territorio *destino, char corJogador) {
    if (origem == NULL || destino == NULL) {
        return;
    }

    printf("\nAtacando de %s (%d tropas) -> %s (%d tropas, cor %c)\n",
           origem->nome, origem->tropas,
           destino->nome, destino->tropas, destino->corExercito);

    int dadoAtq = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;

    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtq, dadoDef);

    if (dadoAtq > dadoDef) {
        destino->tropas--;
        printf("Ataque bem-sucedido! Defender perde 1 tropa.\n");

        if (destino->tropas <= 0) {
            printf("Voce conquistou o territorio %s!\n", destino->nome);
            destino->corExercito = corJogador;
            destino->tropas      = 1;      // Move 1 tropa do territorio de origem
            origem->tropas--;
        }
    } else {
        origem->tropas--;
        printf("Ataque fracassou. Voce perdeu 1 tropa em %s.\n", origem->nome);
    }
}

// ---------------------------------------------------------------------
// sortearMissao()
// ---------------------------------------------------------------------
int sortearMissao(void) {
    // Sorteia uma missao entre 0 e NUM_MISSOES-1
    return rand() % NUM_MISSOES;
}

// ---------------------------------------------------------------------
// verificarVitoria()
// ---------------------------------------------------------------------
int verificarVitoria(const Territorio *mapa, int qtdTerritorios,
                     char corJogador, int idMissao) {
    int countJogador = 0;
    int countTerritoriosComMuitasTropas = 0;
    int existeVermelho = 0; // usado na missao de destruir vermelho

    for (int i = 0; i < qtdTerritorios; i++) {
        if (mapa[i].corExercito == corJogador) {
            countJogador++;
            if (mapa[i].tropas >= 5) {
                countTerritoriosComMuitasTropas++;
            }
        }
        if (mapa[i].corExercito == 'R') {
            existeVermelho = 1;
        }
    }

    switch (idMissao) {
        case 0:
            // Controlar pelo menos 5 territorios
            return (countJogador >= 5);

        case 1:
            // Controlar todos os territorios
            return (countJogador == qtdTerritorios);

        case 2:
            // Destruir completamente o exercito Vermelho
            return (existeVermelho == 0);

        case 3:
            // Ter ao menos 3 territorios com 5+ tropas
            return (countTerritoriosComMuitasTropas >= 3);

        default:
            return 0;
    }
}

// ---------------------------------------------------------------------
// limparBufferEntrada()
// ---------------------------------------------------------------------
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // descarta caracteres restantes
    }
}
