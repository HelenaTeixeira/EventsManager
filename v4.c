/*Helena Teixeira 93720 LEIC-T*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <values.h>

#define CAR 64		/*constante de caracteres da descricao dos eventos*/

#define Sala(N) S[N-1]	/*Macro por causa das salas serem de 1 a 10*/
/*
 * Estrutura Evento: strings na descricao do evento e participantes do mesmo
 * data do evento,inicio do evento,duracao e sala onde se encontra
 * guardados como inteiros
*/
typedef struct Evento{
	char descricao[CAR];
	int data,inicio,duracao,sala;
	char participantes[4][CAR];
}Evento;
/*
 * Estrutura Sala: caracteristicas como o numero da sala, quantos eventos estao na sala
 * e tem um array com Eventos
*/
typedef struct Sala{
	int num;
	Evento eventos[100];
	int num_eventos;
}Sala;
/*variavel global: um array de 10 salas*/
Sala S[10];
/* funcao inicializa(): inicializa as salas com o numero correspondente e poe
 * o numero de eventos em cada a 0
*/
void inicializa(){
	int i;
	for (i=1;i<=10;i++){
		Sala(i).num=i;
		Sala(i).num_eventos =0;
	}
}
/*
 * funcao ler_input(array de pointers):funcao que busca o input dado e o separa
 * o conteudo pelos':' pondo os respetivos pointers no array recebido
*/
void ler_input(char *token[9]){				
	int i;
	static char input[1000];
	fgets(input,1000, stdin);
	
	token[0]=strtok(input,":\n");
	for (i=1;i<9;i++){
		token[i]=strtok(NULL, ":\n");
	}
}

/*funcao convert_hora(string): transforma o inicio do evento em minutos*/
int convert_hora(char inicioString[]){
	int inicio,hora;
	inicio = atoi(inicioString);
	hora = (inicio/100)*60 + (inicio%100);
	return hora;
}
/*
 * funcao convert_data(string): transforma a data do evento em minutos
 * assumindo meses c 31 dias (anos com 372 dias)
*/
long convert_data(char dataString[]){			
	long data,res=0;
	data = atoi(dataString);
	res = res +(data/1000000)*1440;
	data = data%1000000;
	res = res +(data/10000)*44640;
	data = data%10000;
	res =res +data*535680;
	res = res - 1440 - 44640;		/*acertar porque o modulo vai de 0 a 30*/
	return res;
}
/*
 * funcao e_simultaneo(Evento e1,Evento e2):compara se dois eventos sao no mesmo dia
 * a mesma hora. 1 se coincidirem, 0 caso contrario
 */
int e_simultaneo(Evento e1,Evento e2){
	int inicio1,inicio2,fim1,fim2;
	if (e1.data != e2.data)
		return 0;
	inicio1= e1.inicio;
	fim1= e1.inicio+ e1.duracao;
	inicio2= e2.inicio;
	fim2= e2.inicio + e2.duracao;
	if (inicio2<=inicio1 && inicio1< fim2)
		return 1;
	if (inicio2<fim1 && fim1<=fim2)
		return 1;
	if (inicio1<=inicio2 && fim1>= fim2)
		return 1;
	return 0;
}
/*
 * funcao check_ocupado(Sala,Evento): ve se ha algum evento simultaneo ao recebido em 
 * qualquer sala
*/
int check_ocupado(Sala s,Evento e){
	int i, size;
	size = s.num_eventos;
	for(i=0;i<size;i++)
		if (e_simultaneo(s.eventos[i],e))
			return 1;
	return 0;
}
/*
 * funcao check_participante(evento, participante): ve se o participante recebido 
 * coincide com os do evento recebido
*/
int check_participante(Evento e,char participante[CAR]){
	int i;
	for (i=0;i<4;i++){
		if (e.participantes[i][0]=='\0')
			return 0;
		if (strcmp(e.participantes[i],participante)==0)
			return 1;
	}
	return 0;
}
/*
 * funcao analisa_participante(evento,participante): ve se o participante esta em 
 * dois sitios ao mesmo tempo
*/
int analisa_participante(Evento evento, char participante[CAR]){
	int x,y;
	for (x=1; x<=10;x++){
		for (y=0;y<Sala(x).num_eventos;y++){
			if(e_simultaneo(evento,Sala(x).eventos[y])){
				if(check_participante(Sala(x).eventos[y], participante))
					return 1;
			}
		}
	}
	return 0;
}
/*
 * funcao check_requesitos(Evento): consoante as verificacoes feitas nas funcoes anteriores
 * imprime os erros correspondentes
*/
int check_requesitos(Evento e){
	int x ,f = 0;
	if (check_ocupado(Sala(e.sala),e)){
		printf("Impossivel agendar evento %s. Sala%d ocupada.\n",e.descricao,e.sala);
		return 1;
	}
	for (x=0;x<4;x++){
		if (analisa_participante(e,e.participantes[x])){
			printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n",e.descricao,e.participantes[x]);
			f = 1;
		} 
	}
	if(f){
		return 1;		
	}
	return 0;
}
/*
 * funcao check_descricao(cadeia de caracteres,vetor):ve se existe um evento com a descricao recebida,
 * se sim, coloca no vetor o numero da sala em q se encontra e a posicao do evento
*/
int check_descricao(char str[CAR],int coor[2]){
	int i,j,lim;
	for (i=1;i<=10;i++){
		lim = Sala(i).num_eventos;
		for(j=0;j<lim;j++){
			if(strcmp(Sala(i).eventos[j].descricao,str)==0){
				coor[0] = Sala(i).num;
				coor[1]= j;
				return 0;
			}
		}
	}
	printf("Evento %s inexistente.\n",str);
	return 1;
}
/*funcao adiciona_evento(Evento): funcao q acrescenta um evento ao array de eventos da sala*/
void adiciona_evento(Evento e){
	Sala(e.sala).eventos[Sala(e.sala).num_eventos] = e; 
	Sala(e.sala).num_eventos++;
}
/*
 * funcao adiciona(): funcao q inicializa um evento com as caracteristicas recebidas no input  
 * e se for possivel adicionar, chama a função que adiciona o evento
*/
void adiciona(){
	char *input[9];
	int x;
	Evento e;
	ler_input(input);
	strcpy(e.descricao,input[0]);

	e.data = convert_data(input[1]);
	e.inicio=convert_hora(input[2]);
	e.duracao=atoi(input[3]);
	e.sala=atoi(input[4]);
	for (x=0;x<4;x++)
		if (input[5+x]== NULL)
			strcpy(e.participantes[x],"\0");
		else
			strcpy(e.participantes[x],input[x+5]);
	if(check_requesitos(e))
		return;
	adiciona_evento(e);

}
/*
 * funcao delete_evento(num sala, posicao evento): "apaga" um evento, ou seja,
 * faz shift left com os eventos das posicoes seguintes ao a apagar;
 *reduz tambem o contador de eventos
*/
void delete_evento(int loc_sala,int loc_evento){
	int x, s = Sala(loc_sala).num_eventos;
	for (x=loc_evento;x<s;x++)
		Sala(loc_sala).eventos[x] = Sala(loc_sala).eventos[x+1];
	Sala(loc_sala).num_eventos--;
}
/*funcao apagar(): busca qual o evento a apagar e apaga*/
void apagar(){
	char *input[9];
	int coor[2];
	ler_input(input);
	if (!check_descricao(input[0],coor)){
		delete_evento(coor[0],coor[1]);
		return;
	}
}
/*
 * funcao viceversa(Evento): transforma a data em minutos para o formato inicial
 * do input para ser impresso
*/
int viceversa(Evento e){
	int data,dias,mes,ano;
	data =e.data/1440;
	dias = data% 31 +1;
	data /=31;
	mes = data%12 +1;
	data= data/12;
	ano = data;
	data = dias*1000000 + mes*10000 + ano;
	return data;
}
/*
 * funcao viceversa_h(Evento): tranforma o inicio em minutos para o formato inicial
 * do input para ser impresso
*/
int viceversa_h(Evento e){
	int inicio,minutos;
	inicio = e.inicio;
	minutos=inicio%60;
	inicio/=60;
	inicio=inicio*100 + minutos;
	return inicio;
}
/*funcao representa(evento): imprime as caracteristicas do evento da forma pedida*/
void representa(Evento e){
	int x,data,inicio;
	data =viceversa(e);
	inicio =viceversa_h(e);
	printf("%s %08d %04d %d Sala%d %s\n*",e.descricao,data,inicio,e.duracao,e.sala,e.participantes[0]);
	for(x=1;x<4 && e.participantes[x][0]!='\0'; x++){
		printf(" %s",e.participantes[x]);
	}
	printf("\n");
}
/*funcao insertion_sort(num sala): organiza os eventos cronologicamente de uma dada sala */
void insertion_sort(int s){
	Evento valor_a_inserir;
	int pos_escolhida,i;
	for (i=1;i<Sala(s).num_eventos;i++){
		valor_a_inserir=Sala(s).eventos[i];
		pos_escolhida=i;
		while (pos_escolhida>0 && (Sala(s).eventos[pos_escolhida-1].data)+(Sala(s).eventos[pos_escolhida-1].inicio)>
								  valor_a_inserir.data+valor_a_inserir.inicio) {
			Sala(s).eventos[pos_escolhida]=Sala(s).eventos[pos_escolhida-1];
			pos_escolhida--;
		}
		if(pos_escolhida!=i) {
			Sala(s).eventos[pos_escolhida]=valor_a_inserir;
		}
	}
}
/*
 * funcao listagem_sala():consoante o input(num sala) dado, chama a funcao que
 * organiza os eventos nessa sala e representa-os
*/
void listagem_sala(){
	char *input[9];
	int i,s;
	ler_input(input);
	
	s = atoi(input[0]);
	insertion_sort(s);
	for (i=0;i<Sala(s).num_eventos;i++)
		representa(Sala(s).eventos[i]);
}
/*
 * funcao change_inicio():recebe o novo inicio, vai buscar o evento com a descricao dada e faz um backup;
 * cria um evento temporario com o novo inicio e ve se é possivel adicionar;
 * se sim adiciona, se nao adiciona o backup outravez
*/
void change_inicio(){				
	char *input[9];
	int coor[2],n,antigo_inicio;
	Evento temporario;
	ler_input(input);

	n= check_descricao(input[0],coor);
	if(n)
		return;
	antigo_inicio=Sala(coor[0]).eventos[coor[1]].inicio;

	temporario = Sala(coor[0]).eventos[coor[1]];
	delete_evento(coor[0],coor[1]);
	temporario.inicio=convert_hora(input[1]);
	if(!check_requesitos(temporario)){
		adiciona_evento(temporario);
		return;
	}
	temporario.inicio=antigo_inicio;
	adiciona_evento(temporario);
}
/*
 * funcao change_duracao():recebe a nova duracao, vai buscar o evento com a descricao dada e faz um backup;
 * cria um evento temporario com a nova duracao e ve se é possivel adicionar;
 * se sim adiciona, se nao adiciona o backup outravez
*/
void change_duracao(){
	char *input[9];
	int n, coor[2],duracao_antiga;
	Evento temporario;
	ler_input(input);
	n= check_descricao(input[0],coor);
	if(n)
		return;
	duracao_antiga =Sala(coor[0]).eventos[coor[1]].duracao;
	
	temporario =Sala(coor[0]).eventos[coor[1]];
	delete_evento(coor[0],coor[1]);
	temporario.duracao=atoi(input[1]);
	if( !check_requesitos(temporario)){
		adiciona_evento(temporario);
		return;
	}
	temporario.duracao=duracao_antiga;
	adiciona_evento(temporario);
}
/*
 * funcao change_sala():recebe a nova sala, vai buscar o evento com a descricao dada e faz um backup;
 * cria um evento temporario com a nova sala e ve se é possivel adicionar;
 * se sim adiciona, se nao adiciona o backup outravez
*/
void change_sala(){
	char *input[9];
	int n, coor[2],sala_antiga;
	Evento temporario;
	ler_input(input);		/*input[0] c descricao
							input[1] c sala*/
	n= check_descricao(input[0],coor);
	if(n)
		return;
	sala_antiga=Sala(coor[0]).eventos[coor[1]].sala;
	
	temporario =Sala(coor[0]).eventos[coor[1]];
	delete_evento(coor[0],coor[1]);
	temporario.sala=atoi(input[1]);
	if(!check_requesitos(temporario)){
		adiciona_evento(temporario);
		return;
	}
	temporario.sala=sala_antiga;
	adiciona_evento(temporario);
}
/*
 * funcao add_participante():recebe participante e vai buscar o evento com a descricao dada;
 * ve se o evento ja esta cheio ou nao;
 * se nao chama a funcao que ve se e possivel adicionar o participante nesse evento;
 *se sim adiciona, se nao da erro
*/
void add_participante(){
	char *input[9];
	int n,x,coor[2];
	ler_input(input);

	n= check_descricao(input[0],coor);
	if(!n){
		for(x=0;x<4;x++){
			if(strcmp(Sala(coor[0]).eventos[coor[1]].participantes[x],input[1])==0)
				return;
		}
		if (Sala(coor[0]).eventos[coor[1]].participantes[3][0]!='\0'){
			printf("Impossivel adicionar participante. Evento %s ja tem 3 participantes.\n",input[0]);
			return;
		}
		if (!analisa_participante(Sala(coor[0]).eventos[coor[1]],input[1])){
			for (x=0;x<4;x++){
				if(Sala(coor[0]).eventos[coor[1]].participantes[x][0]=='\0'){
					strcpy(Sala(coor[0]).eventos[coor[1]].participantes[x],input[1]);
					return;
				}
			}
		}
		else
			printf("Impossivel adicionar participante. Participante %s tem um evento sobreposto.\n",input[1]);

	}
}/*
 * funcao de_participante():recebe participante e vai buscar o evento com a descricao dada
 * procura o participante no evento e ve se è possivel remove lo, ou seja,
 * ve se n é o responsavel e se ha pelo menos mais participante
*/
void del_participante(){
	char *input[9];
	int n,x, coor[2];
	ler_input(input);
	n= check_descricao(input[0],coor);

	if(!n){
		for(x=1;x<4;x++){					
			if(strcmp(Sala(coor[0]).eventos[coor[1]].participantes[x],input[1])==0)
				break;
		}
		if (x==4)
			return;
		if (Sala(coor[0]).eventos[coor[1]].participantes[2][0]=='\0'){
			printf("Impossivel remover participante. Participante %s e o unico participante no evento %s.\n",input[1],input[0]);
			return;
		}
		for (;x<3;x++){
			strcpy(Sala(coor[0]).eventos[coor[1]].participantes[x],Sala(coor[0]).eventos[coor[1]].participantes[x+1]);
		}
		Sala(coor[0]).eventos[coor[1]].participantes[x][0]='\0';
	}
}
/*funcao num_eventos_total():retorna o numero de eventos q existem ao todo nas salas todas*/
int num_eventos_total(){
	int x,soma=0;
	for (x=1;x<=10;x++)
		soma = soma + Sala(x).num_eventos;
	return soma;
}
/*funcao sort(): funcao que organiza os eventos de todas as salas cronologicamente*/
void sort(){
	int x,y,cont=0,cont1=0, a = num_eventos_total();
	long data_1, data_2, data_3 = 0;
	Evento menor[10];
	while(cont<a){
		cont1=0;
		data_1 = MAXLONG;
		for(x=1;x<=10;x++){
			for(y=0;y<Sala(x).num_eventos;y++){
				data_2 = Sala(x).eventos[y].data+Sala(x).eventos[y].inicio;
				if(data_2 < data_1 && data_2 > data_3){
					menor[0] = Sala(x).eventos[y];
					cont1=1;
					data_1 = data_2;
				}
				else if (data_2 == data_1){
					menor[cont1]=Sala(x).eventos[y];
					cont1++;
				}
			}
		}
		data_3 = data_1;
		for(x=0;x<cont1;x++)		
			representa(menor[x]);
		cont++;
	}
}
/*
 * ciclo principal onde consoante a primeira letra do comando dado no input 
 * direciona para a funcao que faz a acao pretendida
*/
int main(){
	int r = 1;		/*variavel que manda terminar o ciclo*/
	char x;
	inicializa();
	while(r) {
		switch(x= getchar()){
			case 'a':
				getchar();
				adiciona();
				break;
			case 'r':
				getchar();
				apagar();
				break;
			case 's':
				getchar();
				listagem_sala();
				break;
			case 'i':
				getchar();
				change_inicio();
				break;
			case 't':
				getchar();
				change_duracao();
				break;
			case 'm':
				getchar();
				change_sala();
				break;
			case 'A':
				getchar();
				add_participante();
				break;
			case 'R':
				getchar();
				del_participante();
				break;
			case 'l':
				getchar();
				sort();
				break;
			case 'x':
				r=0;
				break;
			default:
				break;
		}

	}
	return 0;
}