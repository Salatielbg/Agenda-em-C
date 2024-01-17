#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Estruturar um contato com id, nome,telefone. Cada um com sua quantidade maxima de dados
typedef struct {
    int id;
    char nome[100];
    char telefone[15];
} Contato;

int proximoId; // variável global para gerir o id de contato

//Funções de uso global:
void aguardarTecla() {
    printf("\n\tPressione uma tecla para continuar...");
    getchar(); 
    getchar(); 
}

void limparTela() {
    system("cls"); // só funciona em windons para limpesa de tela
}

void telainicial(){
    printf("============Agenda de contatos do Patinho============\n\n\n\n");
    printf("\n\tPressione uma tecla para continuar...");
    getchar();
}

void telalixeira(){
    printf("============Lixeira de contatos do Patinho============\n\n\n\n");
    printf("\n\tPressione uma tecla para continuar...");
    getchar();
    getchar();
}


//Funções arquivo cadastro:
void carregarProximoId() {
    FILE *arquivoId = fopen("last_id.txt", "r");
    if (arquivoId != NULL) {
        fscanf(arquivoId, "%d", &proximoId);
        fclose(arquivoId);
    } else {
        proximoId = 1; // Caso o arquivo não exista, comece com o ID 1
    }
}

void salvarProximoId() {
    FILE *arquivoId = fopen("last_id.txt", "w");
    if (arquivoId != NULL) {
        fprintf(arquivoId, "%d", proximoId);
        fclose(arquivoId);
    }
}

void adicionarContato() {
    carregarProximoId(); 
    FILE *arquivo = fopen("agenda.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Contato novoContato;

    novoContato.id = proximoId++; // Atribui o ID e incrementa para o próximo

    printf("\nDigite o novo nome: ");
    getchar();
    fgets(novoContato.nome, 100, stdin);
    novoContato.nome[strcspn(novoContato.nome, "\n")] = '\0';

    printf("\nDigite o telefone: ");
    fgets(novoContato.telefone, 15, stdin);
    novoContato.telefone[strcspn(novoContato.telefone, "\n")] = '\0';
    
    fprintf(arquivo, "%d;%s;%s\n", novoContato.id, novoContato.nome, novoContato.telefone);

    //Fecha os arquivos abertos
    fclose(arquivo);
    salvarProximoId(); // Salva o próximo ID no arquivo
}

void imprimirContatos() {
    FILE *arquivo = fopen("agenda.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Contato contato;

    printf("============\tLISTA DE CONTATOS\t============\n");

    while (fscanf(arquivo, "%d;%[^;];%[^\n]\n", &contato.id, contato.nome, contato.telefone) == 3) {
        printf("\n\n============\tCodigo contato: %d\t============\n\n", contato.id);
        printf("\t\tNome: %s;\n\t\tTelefone: %s.\n", contato.nome, contato.telefone);
    }

    //Fecha os arquivos abertos
    aguardarTecla();
    fclose(arquivo);
}

void apagarContato(int id) {
    FILE *arquivo = fopen("agenda.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    FILE *novoArquivo = fopen("temp.txt", "w");
    if (novoArquivo == NULL) {
        printf("Erro ao abrir o arquivo temporario.\n");
        fclose(arquivo);
        return;
    }

    FILE *lixeira = fopen("lixeira.txt", "a"); 
    if (lixeira == NULL) {
        printf("Erro ao abrir o arquivo de lixeira.\n");
        fclose(arquivo);
        fclose(novoArquivo);
        return;
    }

    Contato contato;
    int contatoEncontrado = 0; // Flag para verificar se o contato foi encontrado

    while (fscanf(arquivo, "%d;%[^;];%[^\n]\n", &contato.id, contato.nome, contato.telefone) == 3) {
        if (contato.id == id) {
            contatoEncontrado = 1;

            // Verifica se o ID já existe na lixeira antes de adicionar
            int idExistente = 0;
            rewind(lixeira); // Volta ao início da lixeira
            while (fscanf(lixeira, "%d;%*[^;];%*[^\n]\n", &idExistente) == 1) {
                if (idExistente == id) {
                    idExistente = 1;
                    break;
                }
            }

            // Adiciona o contato à lixeira apenas se o ID não existir lá
            if (!idExistente) {
                fprintf(lixeira, "%d;%s;%s\n", contato.id, contato.nome, contato.telefone);
            }
        } else {
            fprintf(novoArquivo, "%d;%s;%s\n", contato.id, contato.nome, contato.telefone);
        }
    }

    //Fecha os arquivos abertos
    fclose(arquivo);
    fclose(novoArquivo);
    fclose(lixeira);

    // Contingencia de erro ou sucesso
    if (!contatoEncontrado) {
        printf("\nContato com ID %d nao encontrado. Nenhum contato removido.\n", id);
    } else {
        remove("agenda.txt");
        rename("temp.txt", "agenda.txt");
        printf("\nContato com ID %d removido e movido para a lixeira.\n", id);
    }
    aguardarTecla();
}

void editarContato(int id) {
    FILE *arquivo = fopen("agenda.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    FILE *novoArquivo = fopen("temp.txt", "w");
    if (novoArquivo == NULL) {
        printf("Erro ao abrir o arquivo temporário.\n");
        fclose(arquivo);
        return;
    }

    Contato contato;
    int contatoEncontrado = 0;

    while (fscanf(arquivo, "%d;%[^;];%[^\n]\n", &contato.id, contato.nome, contato.telefone) == 3) {
        if (contato.id == id) {
            contatoEncontrado = 1;
            printf("\nDigite o novo nome: ");
            getchar();
            fgets(contato.nome, 100, stdin);
            contato.nome[strcspn(contato.nome, "\n")] = '\0';

            printf("\nDigite o novo telefone: ");
            fgets(contato.telefone, 15, stdin);
            contato.telefone[strcspn(contato.telefone, "\n")] = '\0';
        }
        fprintf(novoArquivo, "%d;%s;%s\n", contato.id, contato.nome, contato.telefone);
    }

    fclose(arquivo);
    fclose(novoArquivo);

    // Contingencia de erro ou sucesso
    if (!contatoEncontrado) {
        printf("\nContato com ID %d nao encontrado. Nenhum contato editado.\n", id);
        aguardarTecla();
        remove("temp.txt"); // Exclui o arquivo temporário se nenhum contato foi encontrado
    } else {
        printf("\nContato com ID %d Editado.\n", id);
        aguardarTecla();
        remove("agenda.txt"); // Exclui o arquivo original
        rename("temp.txt", "agenda.txt"); // Renomeia o arquivo temporário para o original
    }
}



//Funções arquivo lixeira:
void recuperarContato(int id) {
    FILE *lixeira = fopen("lixeira.txt", "r");
    if (lixeira == NULL) {
        printf("Erro ao abrir o arquivo de lixeira.\n");
    }

    FILE *agenda = fopen("agenda.txt", "a");
    if (agenda == NULL) {
        printf("Erro ao abrir o arquivo de agenda.\n");
        fclose(lixeira);
    }

    FILE *tempArquivo = fopen("templixeira.txt", "w");
    if (tempArquivo == NULL) {
        printf("Erro ao abrir o arquivo temporario.\n");
        fclose(lixeira);
        fclose(agenda);
    }

    Contato contato;
    int contatoEncontrado = 0; // Flag para verificar se o contato foi encontrado

     while (fscanf(lixeira, "%d;%[^;];%[^\n]\n", &contato.id, contato.nome, contato.telefone) == 3) {
        if (contato.id == id) {
            contatoEncontrado = 1;

            // Verifica se o ID já existe na agenda antes de adicionar
            int idExistente = 0;
            rewind(agenda); // Volta ao início da agenda
            while (fscanf(agenda, "%d;%*[^;];%*[^\n]\n", &idExistente) == 1) {
                if (idExistente == id) {
                    idExistente = 1;
                    break;
                }
            }
            // Adiciona o contato à agenda apenas se o ID não existir lá
            if (!idExistente) {
                fprintf(agenda, "%d;%s;%s\n", contato.id, contato.nome, contato.telefone);
            }
        } else {
            fprintf(tempArquivo, "%d;%s;%s\n", contato.id, contato.nome, contato.telefone);
        }
    }
    //Fecha os arquivos abertos
    fclose(agenda);
    fclose(tempArquivo);
    fclose(lixeira);

    // Contingencia de erro ou sucesso
    if (!contatoEncontrado) {
        printf("\nContato com ID %d nao encontrado. Nenhum contato removido.\n", id);
    } else {
        remove("lixeira.txt");
        rename("templixeira.txt", "lixeira.txt"); // Corrigido para renomear o arquivo temporário corretamente
        printf("\nContato com ID %d removido e movido para a lixeira.\n", id);
    }
    aguardarTecla();
}

void apagarContatoLixeira(int id) {
    FILE *lixeira = fopen("lixeira.txt", "r");
    if (lixeira == NULL) {
        printf("Erro ao abrir o lixeira.\n");
        return;
    }

    FILE *tempFile = fopen("templixeira.txt", "w");
    if (tempFile == NULL) {
        printf("Erro ao abrir o arquivo temporario.\n");
        fclose(lixeira);
        return;
    }

    Contato contato;
    int contatoEncontrado = 0; // Flag para verificar se o contato foi encontrado

    while (fscanf(lixeira, "%d;%[^;];%[^\n]\n", &contato.id, contato.nome, contato.telefone) == 3) {
        if (contato.id != id) {
            // Mantém os contatos que não devem ser apagados na lixeira
            fprintf(tempFile, "%d;%s;%s\n", contato.id, contato.nome, contato.telefone);
        } else {
            contatoEncontrado = 1; // Marcamos que o contato foi encontrado
            printf("Contato removido da lixeira com sucesso.\n");
        }
    }
    //Fecha os arquivos abertos
    fclose(tempFile);
    fclose(lixeira);

    // Contingencia de erro ou sucesso  
    if (!contatoEncontrado) {
        printf("\nContato com ID %d nao encontrado. Nenhum contato removido.\n", id);
    } else {
        remove("lixeira.txt");
        rename("templixeira.txt", "lixeira.txt"); // Corrigido para renomear o arquivo temporário corretamente
        printf("\nContato com ID %d removido e movido para a lixeira.\n", id);
    }
    aguardarTecla();
}

void relatoriolixeira(){
    FILE *lixeira = fopen("lixeira.txt", "r");
    if (lixeira == NULL) {
        printf("Erro ao abrir o arquivo de lixeira.\n");
        return;
    }


    Contato contato;
    

    printf("============\tLISTA NA LIXEIRA\t============\n");

    while (fscanf(lixeira, "%d;%[^;];%[^\n]\n", &contato.id, contato.nome, contato.telefone) == 3) {
        printf("\n\n============\tCodigo contato: %d\t============\n", contato.id);
        printf("\t\tNome: %s;\n\t\tTelefone: %s.\n", contato.nome, contato.telefone);
    }
    //Fecha os arquivos abertos
    fclose(lixeira);
    aguardarTecla();
}

void abrirlixeira(){
    telalixeira();
    int opcao;
    int idLixeira; 

    while (1) {
        limparTela();
        printf("1 - Recuperar contato\n");
        printf("2 - Apagar Contato\n");
        printf("3 - Relatorio\n");
        printf("4 - Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
           case 1:
                printf("\nDigite o ID do contato a ser recuperado: ");
                scanf("%d", &idLixeira);
                recuperarContato(idLixeira);
                break;

            case 2:
                printf("\nDigite o ID do contato a ser apagado: ");
                scanf("%d", &idLixeira);
                apagarContatoLixeira(idLixeira);
            break;

            case 3:
                relatoriolixeira();
            break;

            case 4:
                return; // Volte ao menu principal
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                getchar(); 
                getchar();
        }
    }
}


//principal
int main() {
    int id;
    int opcao;

    // Exibe a tela inicial do programa
    telainicial();

    // Loop principal do programa
    while (1) {  // Alterado para 1 para que o loop continue indefinidamente até a escolha de sair
        limparTela();  // Limpa a tela do console

        // Exibe as opções do menu
        printf("1 - Adicionar Contato\n");
        printf("2 - Imprimir Contatos\n");
        printf("3 - Apagar Contato\n");
        printf("4 - Editar Contato\n");
        printf("5 - Lixeira\n");
        printf("6 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                //Chama função para cadastrar contato
                adicionarContato();
                break;

            case 2:
                //Limpa a tela    
                limparTela();
                //Chama o relatorio de contatos
                imprimirContatos();
                break;

            case 3:
                // Solicita o ID do contato a ser apagado
                printf("\nDigite o ID do contato a ser apagado: ");
                scanf("%d", &id);
                apagarContato(id);
                break;

            case 4:
                // Solicita o ID do contato a ser editado
                printf("\nDigite o ID do contato a ser editado: ");
                scanf("%d", &id);
                editarContato(id);
                break;

            case 5:
                //Limpa a tela  
                limparTela();
                //Abre a lixeira de contatos
                abrirlixeira();
                break;

            case 6:
                // Encerra o programa
                return 0;

            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                getchar(); 
                getchar(); 
        }
    }

    // Nunca deverá chegar aqui, pois o programa encerra no caso 6
    return 0;
}

