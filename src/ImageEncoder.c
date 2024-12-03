#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void exibirAjuda() {
    printf("Uso: ImageEncoder [-? | -m | -f ARQ]\n");
    printf("Codifica imagens binárias dadas em arquivos PBM ou por dados informados manualmente.\n");
    printf("Argumentos:\n");
    printf("-?, --help  : apresenta essa orientação na tela.\n");
    printf("-m, --manual: ativa o modo de entrada manual.\n");
    printf("-f, --file  : considera a imagem representada no arquivo PBM.\n");
}

char verificarHomogeneidade(int **imagem, int largura, int altura) {
    int valor = imagem[0][0];
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            if (imagem[i][j] != valor) {
                return 'X';
            }
        }
    }
    return (valor == 0) ? 'B' : 'P';
}

void dividirQuadrantes(int **imagem, int largura, int altura, int ***q1, int ***q2, int ***q3, int ***q4,
                       int *lq1, int *hq1, int *lq2, int *hq2, int *lq3, int *hq3, int *lq4, int *hq4) {
    int midAltura = altura / 2;
    int midLargura = largura / 2;

    *hq1 = midAltura + (altura % 2);
    *lq1 = midLargura + (largura % 2);
    *hq2 = *hq1;
    *lq2 = largura - *lq1;
    *hq3 = altura - *hq1;
    *lq3 = *lq1;
    *hq4 = *hq3;
    *lq4 = *lq2;

    *q1 = malloc(*hq1 * sizeof(int *));
    *q2 = malloc(*hq2 * sizeof(int *));
    *q3 = malloc(*hq3 * sizeof(int *));
    *q4 = malloc(*hq4 * sizeof(int *));
    for (int i = 0; i < *hq1; i++) (*q1)[i] = malloc(*lq1 * sizeof(int));
    for (int i = 0; i < *hq2; i++) (*q2)[i] = malloc(*lq2 * sizeof(int));
    for (int i = 0; i < *hq3; i++) (*q3)[i] = malloc(*lq3 * sizeof(int));
    for (int i = 0; i < *hq4; i++) (*q4)[i] = malloc(*lq4 * sizeof(int));

    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            if (i < *hq1 && j < *lq1)
                (*q1)[i][j] = imagem[i][j];
            else if (i < *hq1)
                (*q2)[i][j - *lq1] = imagem[i][j];
            else if (j < *lq1)
                (*q3)[i - *hq1][j] = imagem[i][j];
            else
                (*q4)[i - *hq1][j - *lq1] = imagem[i][j];
        }
    }
}

void codificarImagem(int **imagem, int largura, int altura, char *codigo) {
    char resultado = verificarHomogeneidade(imagem, largura, altura);
    if (resultado != 'X') {
        strncat(codigo, &resultado, 1);
        return;
    }

    strcat(codigo, "X");

    int **q1, **q2, **q3, **q4;
    int lq1, hq1, lq2, hq2, lq3, hq3, lq4, hq4;
    dividirQuadrantes(imagem, largura, altura, &q1, &q2, &q3, &q4, &lq1, &hq1, &lq2, &hq2, &lq3, &hq3, &lq4, &hq4);

    codificarImagem(q1, lq1, hq1, codigo);
    codificarImagem(q2, lq2, hq2, codigo);
    codificarImagem(q3, lq3, hq3, codigo);
    codificarImagem(q4, lq4, hq4, codigo);

    for (int i = 0; i < hq1; i++) free(q1[i]);
    for (int i = 0; i < hq2; i++) free(q2[i]);
    for (int i = 0; i < hq3; i++) free(q3[i]);
    for (int i = 0; i < hq4; i++) free(q4[i]);
    free(q1);
    free(q2);
    free(q3);
    free(q4);
}

int **lerArquivoPBM(const char *nomeArquivo, int *largura, int *altura) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    char tipo[3];
    fscanf(arquivo, "%2s", tipo);
    if (strcmp(tipo, "P1") != 0) {
        printf("Formato de arquivo não suportado.\n");
        exit(1);
    }

    *largura = *altura = 0;
    fscanf(arquivo, "%d %d", largura, altura);

    int **imagem = malloc(*altura * sizeof(int *));
    for (int i = 0; i < *altura; i++) {
        imagem[i] = malloc(*largura * sizeof(int));
        for (int j = 0; j < *largura; j++) {
            fscanf(arquivo, "%d", &imagem[i][j]);
        }
    }

    fclose(arquivo);
    return imagem;
}

int **entradaManual(int *largura, int *altura) {
    printf("Digite a largura e a altura da imagem: ");
    scanf("%d %d", largura, altura);

    int **imagem = malloc(*altura * sizeof(int *));
    for (int i = 0; i < *altura; i++) {
        imagem[i] = malloc(*largura * sizeof(int));
        for (int j = 0; j < *largura; j++) {
            printf("Pixel [%d][%d]: ", i, j);
            scanf("%d", &imagem[i][j]);
        }
    }

    return imagem;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        exibirAjuda();
        return 1;
    }

    int largura, altura;
    int **imagem = NULL;

    if (strcmp(argv[1], "-?") == 0 || strcmp(argv[1], "--help") == 0) {
        exibirAjuda();
        return 0;
    } else if (strcmp(argv[1], "-m") == 0 || strcmp(argv[1], "--manual") == 0) {
        imagem = entradaManual(&largura, &altura);
    } else if (strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--file") == 0) {
        if (argc < 3) {
            printf("Erro: Nome do arquivo não fornecido.\n");
            return 1;
        }
        imagem = lerArquivoPBM(argv[2], &largura, &altura);
    } else {
        printf("Erro: Argumento inválido.\n");
        exibirAjuda();
        return 1;
    }

    char codigo[1024 * 768] = "";
    codificarImagem(imagem, largura, altura, codigo);
    printf("Código gerado: %s\n", codigo);

    for (int i = 0; i < altura; i++) free(imagem[i]);
    free(imagem);

    return 0;
}
