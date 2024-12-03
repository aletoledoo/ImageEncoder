# ImageEncoder

Um programa em C que codifica imagens binárias no formato PBM (`P1`) utilizando um algoritmo de divisão recursiva para gerar uma string representando a estrutura da imagem.

## Funcionalidades
- Leitura de arquivos PBM (`P1`).
- Entrada manual de pixels.
- Codificação de imagens com divisões recursivas em quadrantes.
- Geração de string que descreve regiões homogêneas (`B` para branco, `P` para preto) e divisões (`X`).

## Como Usar

### Compilação

```gcc -o ImageEncoder ImageEncoder.c ```

### Execução 

- Ajuda: ``` ./ImageEncoder -? ```
- Entrada manual: ``` ./ImageEncoder -m ```
- Arquivo PBM: ``` ./ImageEncoder -f <arquivo.pbm> ```

## Erros Comuns

- Formato inválido: O arquivo deve começar com P1.
- Dimensões incorretas: Devem ser números positivos.
- Dados incompletos: Todos os pixels precisam estar presentes.

## Autor

- Alexandre Toledo
