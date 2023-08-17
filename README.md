# seg-ciber-sistemas-eletricos
Este repositório contém os códigos em linguagem C de ataques cibernéticos realizados em um simulação de um sistema elétrico com a utilização de um Real-Time Digital Simulator (RTDS) e um IED comercial com o objetivo de se estudar a segurança cibernética nos ambientes de infraestruturas críticas de sistemas elétricos de potência.

## Ataques cibernéticos
A digitalização dos sistemas elétricos, promovida principalmente pelo advento o conjunto de normas 61850 da International Electrotechnical Comission (IEC 61850), trouxeram as ameaças cibernéticas a esses ambientes. Na literatura acadêmica, verifica-se diversos ataques cibernéticos possíveis que tomam proveito das vulnerabilidades do protocolo GOOSE da norma IEC 61850. Esses ataques são o ataque de retransmissão, de injeção de mensagem, de mascaramento e de envenamento [Quincozes et al. 2021].

O ataque de retransmissão é realizado com o reenvio de mensagens legítimas previamente enviadas.  Esse ataque tem o potencial de causar o funcionamento errôneo quando mesangens antigas com comandos realizados em determinado contexto de porteção e controle são reenviadas em outro contexto causando a realização de comandos  [Hong et al. 2014].

O ataque de mascaramento é realizado a partir da captura de uma mensagem previamente enviada e seu conteúdo é modificado para realizar algum objetivo específico do atacante cibernético. Porém, nesse tipo de ataque, os parâmetros de sequenciamento das mensagens do protocolo GOOSE como o sqnum e stnum devem possuir coerência com o tráfego em tempo real, isso com o objetivo de dificultar medidas de detecção de intrusão que venham realizam a verificação do sequenciamento das mensagens [Ustun et al. 2019].

Já o ataque de envenenamento é realizado com o objetivo de degradar a comunicação de rede entre os equipamentos da subestação. Essa degradação pode ser realizada através do inundamento de pacotes GOOSE na rede, causando a negação de serviço pelo mau funcionamento da comunicação [Kush et al. 2014].. 

Neste repositório encontram-se os arquivos dos ataques realizados de Retransmissão, Mascaramento e de Negação de Serviço por inundamento de pacotes GOOSE.


### Simulação

Foi simulado um sistema de proteção de distância de linhas de transmissão entre duas subestações. O sistema foi modelado no RTDS e foram utilizados os seguintes equipamentos físicos: IED SEL-421, de proteção de distância; RTAC SEL-3555 como sistema supervisório (SCADA) e um switch para interfacear com o RTDS. Os ataques cibernéticos foram realizados por uma máquina convencional processador Intel I7-8550U 1.8 GHz e memória RAM 12 GB DDR4 com sistema operacional Windows. A máquina atacante possuía acesso à rede operacional no nível de estação do sistema elétrico simulado. 

A proteção de distância na subestação 1 foi realizada pelo IED real SEL-421, e a proteção de distância na subestação 2 foi realizada por um IED virtual no RTDS. Além disso, foram simulados no RTDS um disjuntor para cada subestação, que recebiam comandos de seus respecitivos IEDs de proteção e enviavam o seu status aos mesmos.

Os ataques de retransmissão, mascaramento e de negação de serviço mostraram as vulnerabilidades descritas na literatura, causando acionamentos incorretos, no caso dos ataques de retransmissão e mascaremntos; e causando o impedimento da execução de comandos, com o ataque de negação de serviço.

### Implementação dos Ataques cibernéticos

Os ataques cibernéticos foram implementados com a utilização da biblioteca open-source libiec61850 (https://libiec61850.com/), que provê interface servidor e cliente para comunicação em protocolos MMS, GOOSE e SV do conjunto de normas IEC 61850 em linguagem C. O autor da biblioteca libiec61850 informa que o objetivo do seu projeto é prover implementação que tenha portabilidade para ser usado em sistemas embarcados e também disponibiliza exemplos de códigos para serem usados como ponto de partida para a criação de aplicações (https://libiec61850.com/about/).

Os exemplos de códigos "goose_observer", "goose_publisher" e "goose_subscriber" e algumas de suas funções serviram como ponto de partida para a criação dos códigos sendo implementados pelos autores as lógicas de funcionamento de cada ataque mencionado na seção anterior. Os códigos estão disponíveis neste repositório com os comentários pertinentes para entedimento das soluções.

A geração dos projetos no programa Visual Studio da Microsoft foi realizada através dos passos a seguir, muitos deles semelhantes à algumas instruções presentes na página https://libiec61850.com/documentation/building-the-library/ de como construir a biblioteca no sistema operaiconal Windows, com exceção de algumas particularidades desse trabalho em questão:

1-Carregar na máquina a última versão da libiec61850 presente na página https://libiec61850.com/downloads/.

2-Instalação na máquina Windows o Visual Studio 2019 Community.

3-Instalação do programa CMAKE.

4-Intalação do programa Winpcap.

5-Para permitir comunicação GOOSE, como orientado na página de contrução da biblioteca libiec61850, foi baixado o pacote de desenvolvimento através do endereço http://www.winpcap.org/install/bin/WpdPack_4_1_2.zip e os arquivos Lib e Include do diretório WpdPack foram inseridos na pasta third_party/winpcap do diretório da libiec61850 carregada no passo 1.

6-Foi criada uma nova pasta no diretório principal da libiec61850 onde os projetos do Visual Studio serão criados nos passos seguintes.

7-Na pasta criada, foram criadas 3 pastas, uma para cada ataque e em cada uma foram acrescentados 3 arquivos: o arquivos do código em c, o arquvio Makefile e o arquivo CMakeList.txt a partir de um dos exemplos da libiec61850. 

8-Os arquivos foram editados de acordo com os nomes dados. Também foram editados os arquivos Makefile e CMakeList.txt do diretório principal da libiec61850 para incluir os nomes das pastas dos projetos a serem criados dos ataques.

9-Para a geração dos projetos, acessou-o o terminal windows a partir do diretório principal da libiec61850 e criou-se uma nova pasta onde os projetos serão carregados.

10-Após a criação da pasta, ela foi acessada através do terminal onde foi executado a seguinte linha de código:

   
   "cmake -G "Visual Studio 16 2019" -A Win32 .."


11-Após isso, os projetos no Visual Studio serão criados na pasta criada no passo 9, podendo a partir de agora serem executados ou seus códigos fonte em c serem editados.



## Referências

Hong, J., Liu, C.-C., and Govindarasu, M. (2014). Detection of cyber intrusions using
network-based multicast messages for substation automation. In ISGT 2014, pages
1–5.

Kush, N., Ahmed, E., Branagan, M., and Foo, E. (2014). Poisoned goose: Exploiting the
goose protocol. volume 149, page 17 – 22.

Quincozes, S. E., Albuquerque, C., Passos, D., and Moss ́e, D. (2021). A survey on
intrusion detection and prevention systems in digital substations. Computer Networks,
184:107679.

Ustun, T. S., Farooq, S. M., and Hussain, S. M. S. (2019). A novel approach for mitigation
of replay and masquerade attacks in smartgrids using iec 61850 standard. IEEE Access,
7:156044–156053.


