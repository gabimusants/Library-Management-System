//Aluna: Gabrielle Muniz Santos

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <malloc.h>
#include <time.h>

int ordem, max; // Valores de t e 2t-1
long totLivros = 0, difLivros = 0;
float tempo;
time_t t_ini, t_fim;

//Struct de livro contendo suas informações
typedef struct livro {
    long codigo;
    char titulo[60];
    long quantidade;
    short status;
} livro;

typedef struct pagina *apontador;
// Struct contendo informações de cada nó (ou página)
typedef struct pagina {
    int n;                 //Quantidade de chaves na página
    livro r[1001];         //Array de structs livro
    apontador p[1001];     //Array de descendentes
} pagina;

//Iniciar uma árvore vazia
void inicializa(apontador *a) {
    *a = NULL;
}

void busca(long codigo, apontador ap) {
    //t_ini = time(NULL);
    long i = 1;

    //Se apontador aponta pra uma página nula significa que o livro não está no sistema
    if(ap == NULL) {
        printf("\t-3\n");
        printf("\tLivro nao existente no sistema!\n");
        return;
    }

    //Procura o intervalo correto do codigo
    while(i < ap->n && codigo > ap->r[i-1].codigo) 
        i++;

    //Se o codigo do livro é igual ao codigo procurado, imprime as informações
    if(codigo == ap->r[i-1].codigo){
        // t_fim = time(NULL);
        // tempo = difftime(t_fim, t_ini);
        // printf("TEMPO: %f\n", tempo);
        printf("\tLivro encontrado!\n\tCódigo: %ld\t", codigo);
        printf("\tNome: %s", ap->r[i-1].titulo);
        printf("\tQuantidade: %ld\tStatus: %hu\n\n", ap->r[i-1].quantidade, ap->r[i-1].status);
        return;
    }

    //Se o codigo procurado é menor, descemos na página esquerda
    if(codigo < ap->r[i-1].codigo)
        busca(codigo, ap->p[i-1]);

    // Se o codigo procurado é maior, descemos na página direita
    else
        busca(codigo, ap->p[i]);     
}

void relatorio(apontador ap) {
    int i = 0;
    if(ap != NULL) {
        while (i < ap->n) {
            relatorio(ap->p[i]);
            printf("\tNome: %s\tCodigo: %ld", ap->r[i].titulo, ap->r[i].codigo);
            printf("\tQuantidade: %ld\tStatus: %hu\n\n", ap->r[i].quantidade, ap->r[i].status);
            i++;
        }
        relatorio(ap->p[i]);
    }
    return;
}

//Faz o incremento da quantidade de livros no sistema
void entrada(long codigo, long n, apontador ap) {
    long i = 1;

    if(ap == NULL) {
        printf("\t-4\n");
        printf("\tLivro nao existente no sistema!\n");
        return;
    }

    while (i < ap->n && codigo > ap->r[i - 1].codigo) 
        i++;

    //Livro foi encontrado
    if(codigo == ap->r[i - 1].codigo) {
        ap->r[i-1].quantidade += n;     //Incrementa sua quantidade
        totLivros += n;                 //Incrementa a quantidade total de livros
        ap->r[i - 1].status = 1;        
        printf("\tIncremento feito com sucesso!\n\n");
        return;
    }

    if(codigo < ap->r[i - 1].codigo)
        entrada(codigo, n, ap->p[i - 1]);
    else
        entrada(codigo, n, ap->p[i]);
    
}

//Faz o decremento da quantidade de livros no sistema
void saida(long codigo, long n, apontador ap) {
    long i = 1;

    if (ap == NULL) {
        printf("\tLivro nao existente no sistema!\n\n");
        return;
    }

    while (i < ap->n && codigo > ap->r[i - 1].codigo) 
        i++;

    //Livro foi encontrado
    if (codigo == ap->r[i - 1].codigo) {
        if(ap->r[i - 1].quantidade < n) {   //Caso numero de decremento seja maior que a quantidade disponível, nada é feito
            printf ("\t-5\n");              //Usuário é avisado que não foi possível realizar a operação
            return;
        } else {
            ap->r[i - 1].quantidade -= n;   //Quantidade é decrementada
            totLivros -= n;                 //Total de livros é decrementado
            if(ap->r[i-1].quantidade == 0)
                ap->r[i-1].status = 0;
            printf("\tDecremento realizado com sucesso!\n\n");
            return;
        }
    }

    if (codigo < ap->r[i - 1].codigo)
        saida(codigo, n, ap->p[i - 1]);
    else
        saida(codigo, n, ap->p[i]);
    
}

//Insere um livro e um ponteiro para filho da direita numa página
void insereNaPagina(apontador ap, livro liv, apontador apdir) {
    int semPos, q;
    q = ap->n;
    semPos = 1;

    while(semPos) {
        //Verifica se o livro pode ser inserido na ultima posição
        if(liv.codigo >= ap->r[q-1].codigo) {
            semPos = 0;
            break;
        }

        //Faz o reposicionamento caso seja necessário
        ap->r[q] = ap->r[q-1];
        ap->p[q+1] = ap->p[q];
        q--;

        if(q < 1)
            semPos = 0;
    }

    //Insere a chave na posição correta
    ap->r[q] = liv;
    ap->p[q+1] = apdir;
    ap->n++;
}

void ins(livro liv, apontador ap, short *cresceu, livro *livRetorno, apontador *apRetorno) {
    long i = 1, j;
    apontador apTemp;

    if(ap == NULL) {    //Critério de parada
        *cresceu = 1;
        *livRetorno = liv;
        *apRetorno = NULL;
        printf("\n\tLivro inserido no sistema!\n\n");
        return;
    }

    while(i < ap->n && liv.codigo > ap->r[i-1].codigo)  //Procuro o intervalo correto
        i++;

    if(liv.codigo == ap->r[i-1].codigo) { //Codigos iguais, livro já contido no sistema
        printf("\tLivro ja esta no sistema!\n");
        printf("\t-1\n");
        *cresceu = 0;
        return;
    }

    if(liv.codigo < ap->r[i-1].codigo) //Código do livro menor que o codigo da posição i-1
        i--;                           //Decremento i para passar o valor do ponteiro correto
    
    ins(liv, ap->p[i], cresceu, livRetorno, apRetorno); //Função recursiva para percorrer a árvore

    if(!*cresceu)
        return;

    if(ap->n < max) { //Verifico se a pagina tem espaço
        insereNaPagina(ap, *livRetorno, *apRetorno);    //Se tem espaço, insiro na página
        *cresceu = 0;                                   //Se tem espaço, árvore não cresce
        return;
    }

    //Página é dividida
    apTemp = (apontador)malloc(sizeof(pagina));     //Crio uma nova página
    apTemp->n = 0;                                  //Inicialmente tem 0 chaves
    apTemp->p[0] = NULL;

    //Inserção das chaves após split em suas devidas páginas
    if(i <= ordem+1) {
        insereNaPagina(apTemp, ap->r[max-1], ap->p[max]);
        ap->n--;
        insereNaPagina(ap, *livRetorno, *apRetorno);
    } else 
        insereNaPagina(apTemp, *livRetorno, *apRetorno);

    for(j = ordem + 2; j <= max; j++)
        insereNaPagina(apTemp, ap->r[j-1], ap->p[j]);

    ap->n = ordem;
    apTemp->p[0] = ap->p[ordem+1];
    *livRetorno = ap->r[ordem];
    *apRetorno = apTemp;
    
}

void insere(livro liv, apontador *ap) {
    //t_ini = time(NULL);
    short cresceu = 0;
    livro livRetorno;
    pagina *apRetorno, *apTemp;
    
    ins(liv, *ap, &cresceu, &livRetorno, &apRetorno);

    if(cresceu) { //Caso a árvore aumente de altura
        apTemp = (pagina *)malloc(sizeof(pagina)); //Uma nova raiz é criada
        apTemp -> n = 1;                           //Só terá 1 chave
        apTemp -> r[0] = livRetorno;               //Dados da chave da raiz
        apTemp -> p[1] = apRetorno;                //Defino qual o filho da direita da raiz
        apTemp -> p[0] = *ap;                      //Filho da esquerda
        *ap = apTemp;                              //Endereço da nova raiz
    }
    difLivros++;                                    //Após inserção, a quantidade de livros diferentes aumenta
    // t_fim = time(NULL);
    // tempo = difftime(t_fim, t_ini);
    // printf("TEMPO: %f\n", tempo);
}

void reconstroiUnderflow(apontador ap, apontador apPai, int posPai, short *diminuiu) {
    pagina *aux;        
    long dispAux, i;

    if(posPai < apPai->n) {     //Página a direita de ap
        aux = apPai->p[posPai+1];
        dispAux = (aux->n - ordem + 1)/2;
        ap->r[ap->n] = apPai->r[posPai];
        ap->p[ap->n + 1] = aux->p[0];
        ap->n++;

        if(dispAux > 0) {       //Transfere de aux pra ap
            for(i = 1; i < dispAux; i++)
                insereNaPagina(ap, aux->r[i-1], aux->p[i]);
            
            apPai->r[posPai] = aux->r[dispAux-1];
            aux->n -= dispAux;

            for(i = 0; i < aux->n; i++)
                aux->r[i] = aux->r[i+dispAux];

            for (i = 0; i < aux->n; i++)
                aux->p[i] = aux->p[i + dispAux];

            *diminuiu = 0;
        } else {                //Caso de fusão, intercala aux em ap
            for(i = 1; i <= ordem; i++)
                insereNaPagina(ap, aux->r[i-1], aux->p[i]);
            free(aux);          //Na fusão, uma pagina é removida

            for(i = posPai + 1; i < apPai->n; i++) {    //É feita a reorganização
                apPai->r[i-1] = apPai->r[i];
                apPai->p[i] = apPai->p[i+1];
            }
             
            apPai->n--;
            if(apPai->n >= ordem)   //Verifica se ocorreu underflow
                *diminuiu = 0;
        }
    } else {                    //Aux é pagina a esquerda de ap
        aux = apPai->p[posPai-1];
        dispAux = (aux->n - ordem + 1)/2;

        for(i = ap->n; i >= 1; i--)
            ap->r[i] = ap->r[i-1];
        ap->r[0] = apPai->r[posPai-1];

        for (i = ap->n; i >= 0; i--)
            ap->p[i+1] = ap->p[i];
        ap->n++;

        if(dispAux > 0) {       //Transfere de aux pra ap
            for(i = 1; i < dispAux; i++)
                insereNaPagina(ap, aux->r[aux->n-i], aux->p[aux->n-i+1]);
            ap->p[0] = aux->p[aux->n - dispAux + 1];
            apPai->r[posPai-1] = aux->r[aux->n - dispAux];
            aux->n -= dispAux;
            *diminuiu = 0;
        } else {                //Caso de fusão, intercala ap em aux
            for(i = 1; i <= ordem; i++)
                insereNaPagina(aux, ap->r[i-1], ap->p[i]);
            free(ap);           // Na fusão, uma pagina é removida
            apPai->n--;

            if(apPai->n >= ordem)
                *diminuiu = 0;
        }
    }
}

//Função que realiza a troca da chave removida pela antecessora
void antecessor(apontador ap, int ind, apontador apPai, short *diminuiu) {
    if(apPai->p[apPai->n] != NULL) {
        antecessor(ap, ind, apPai->p[apPai->n], diminuiu);
        if(*diminuiu)      // Verifica se ocorreu underflow
            reconstroiUnderflow(apPai->p[apPai->n], apPai, apPai->n, diminuiu);
        return;
    }

    ap->r[ind-1] = apPai->r[apPai->n - 1];
    apPai->n--;     //Diminui a quantidade de chaves no nó
    *diminuiu = (apPai->n < ordem); //Verifica underflow
}

void ret(long cod, apontador *ap, short *diminuiu) {
    long i, ind = 1;
    apontador pagina;

    //Critério de parada
    if(*ap == NULL) {
        printf("\t-2\n");
        printf("\tLivro nao esta no sistema!\n");
        *diminuiu = 0;
        return;
    }

    pagina = *ap;
    while(ind < pagina->n && cod > pagina->r[ind-1].codigo) //Procuro o intervalo correto
        ind++;

    if(cod == pagina->r[ind-1].codigo && pagina->r[ind-1].quantidade > 0) { //Caso o codigo procurado seja o mesmo do livro e a quantidade do livro seja > 0, não é possível realizar a remoção
        printf("-2\n");
        return;
    }
    
    if(cod == pagina->r[ind-1].codigo) {        //Codigo do livro a ser removido é igual o código no nó
        if(pagina->p[ind-1] == NULL) {          //Pagina é uma folha
            pagina->n--;                        //Diminui a quantidade de chaves no nó
            *diminuiu = pagina->n < ordem;    //Verifica se ocorreu underflow
            for(i = ind; i <= pagina->n; i++) { //Reorganiza página
                pagina->r[i-1] = pagina->r[i];  
                pagina->p[i] = pagina->p[i + 1];
            }
            difLivros--;
            printf("\tLivro removido com sucesso!\n");
            return;
        }
    
        //Caso a página não seja folha, troco com o antecessor
        antecessor(*ap, ind, pagina->p[ind-1], diminuiu);
        if(*diminuiu)
            reconstroiUnderflow(pagina->p[ind - 1], *ap, ind-1, diminuiu);
        return;
    }

    //Se o codigo procurado é maior, descemos na página direita
    if(cod > pagina->r[ind-1].codigo)
        ind++;

    ret(cod, &pagina->p[ind-1], diminuiu);
    if(*diminuiu)
        reconstroiUnderflow(pagina->p[ind - 1], *ap, ind-1, diminuiu);
}

void retira(long cod, apontador *ap) {
    short diminuiu;
    apontador aux;
    ret(cod, ap, &diminuiu);

    if(diminuiu && (*ap)->n == 0) {     //Arvore tem sua altura diminuída
        aux = *ap;
        *ap = aux->p[0];                //Antiga raiz é eliminada
        free(aux);
    }
}   

int main() {
    int op, num = 0;
    long qnt, cod;
    char nome[100];
    apontador *pag = (apontador*)malloc(sizeof(apontador));
    livro l;

    inicializa(pag);
    printf("Qual valor de t? ");
    scanf("%d", &ordem);
    printf("\n");

    max = 2 * ordem - 1;

    while(1) {
        printf("######################### MENU #########################\n\n");
        printf("\tQual operacao deseja realizar?\n\n");
        printf("\t\t1 - INSERCAO\n\t\t2 - REMOCAO\n\t\t3 - BUSCA\n");
        printf("\t\t4 - ENTRADA\n\t\t5 - SAIDA\n\t\t6 - RELATORIO\n\t\t0 - SAIR\n\n");
        printf("\tOperacao: ");
        scanf("%d", &op);
        switch(op) {
            case 1:
                t_ini = time(NULL);
                printf("\tQual codigo do livro que deseja inserir?\n\t");
                scanf("%ld", &cod);
                l.codigo = cod;
                printf("\tQual nome do livro?\n\t");
                getchar();
                fgets(l.titulo, sizeof(l.titulo), stdin);
                l.quantidade = 0;

                insere(l, pag);
                t_fim = time(NULL);
                tempo = difftime(t_fim, t_ini);
                printf("TEMPO: %f\n", tempo);
                break;

            case 2:
                printf("\tQual codigo do livro que deseja remover?\n\t");
                scanf("%ld", &cod);
                retira(cod, pag);
            break;

            case 3:
                t_ini = time(NULL);
                printf("\tQual codigo do livro que deseja buscar?\n\t");
                scanf("%ld", &cod);
                busca(cod, *pag);
                t_fim = time(NULL);
                tempo = difftime(t_fim, t_ini);
                printf("TEMPO: %f\n", tempo);
                break;

            case 4:
                printf("\tQual a quantidade de entrada?\n\t");
                scanf("%ld", &qnt);
                printf("\tQual codigo do livro?\n\t");
                scanf("%ld", &cod);
                entrada(cod, qnt, *pag);
            break;

            case 5:
                printf("\tQual a quantidade de saida?\n\t");
                scanf("%ld", &qnt);
                printf("\tQual codigo do livro?\n\t");
                scanf("%ld", &cod);
                saida(cod, qnt, *pag);
            break;

            case 6:
                printf("\n\t---------------------------------------------\n");
                printf("\t\t\tRELATORIO");
                printf("\n\t---------------------------------------------\n");
                printf("\tLivros diferentes: %ld\n", difLivros);
                printf("\tTotal de livros: %ld\n\n", totLivros);
                relatorio(*pag);
            break;

            case 0:
                printf("\n\t\tFIM DO PROGRAMA!\n");
                return 0;

            default:
                printf("\tValor inválido!\n");
                printf("\tDigite um valor valido\n");
        }
    }
    return 0;
}