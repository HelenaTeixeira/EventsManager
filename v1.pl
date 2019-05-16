% Helena Teixeira 93720 LEIC-T

:- consult(codigo_comum).
:-consult(puzzles_publicos).
% ----------------------------------------------
% aplica_R1_triplo(Triplo, N_triplo) onde Triplo e
% uma lista de tres elementos e N_triplo e a lista
% resultante de aplicar a regra 1 ao Triplo.
% ----------------------------------------------

% --- aux: contador1(L,N) conta o numero de 1's
% (colocando em N) na lista L---
contador1([],0):-!.
contador1([P|R],N):-
				P==1,!,
				contador1(R,N1),
				N is N1 +1.
contador1([P|R],N):-
				P\==1,!,
				contador1(R,N1),
				N is N1.
% --- aux: contador0(L,N) conta o numero de 0's
% (colocando em N) na lista L---
contador0([],0):-!.
contador0([P|R],N):-
				P==0,!,
				contador0(R,N1),
				N is N1 +1.
contador0([P|R],N):-
				P\==0,!,
				contador0(R,N1),
				N is N1.

% --- aux: completa1(L,Res) dada uma lista L, completa com 1
% a variavel e faz a nova lista Res preenchida---

completa1([],[]):-!.
completa1([P|R],[1|R]):-var(P),!.
completa1([P|R],[P|Res]):-
					nonvar(P),!,
					completa1(R,Res).

% --- aux: completa0(L,Res) dada uma lista L, completa com 0
% a variavel e faz a nova lista Res preenchida---

completa0([],[]):-!.
completa0([P|R],[0|R]):-var(P),!.
completa0([P|R],[P|Res]):-
					nonvar(P),!,
					completa0(R,Res).


aplica_R1_triplo([],[]):-!.
aplica_R1_triplo([P|R],Res):-
						contador0([P|R],N),
						N==2,!,
						completa1([P|R],Res).
aplica_R1_triplo([P|R],Res):-
						contador1([P|R],N),
						N==2,!,
						completa0([P|R],Res).
aplica_R1_triplo([P|R],Res):-
							contador0([P|R],N),
							N==1,!,
							Res =[P|R].
aplica_R1_triplo([P|R],Res):-
							contador1([P|R],N1),
							N1==1,!,
							Res =[P|R].
aplica_R1_triplo([P|R],Res):-
							contador1([P|R],N1),
							contador0([P|R],N0),
							N1==0,N0==0,
							Res = [P|R].

% --------------------------------------------------
% aplica_R1_fila_aux(Fila,N_Fila) onde aplica o predicado aplica_R1_triplo
% a todos os triplos da Fila dada
% --------------------------------------------------

aplica_R1_fila_aux([E1,E2],[E1,E2]):-!.
aplica_R1_fila_aux([E1,E2,E3|R],[H1|Res]):-
								aplica_R1_triplo([E1,E2,E3],[H1,H2,H3]),
								aplica_R1_fila_aux([H2,H3|R],Res).

% ---------------------------------------------------
% aplica_R1_fila(Fila,N_Fila) aplica o aplica_R1_fila_aux a Fila
% ate nao haver mais alteracoes a acontecer
% ---------------------------------------------------


aplica_R1_fila(Lista,Res):- 
							aplica_R1_fila_aux(Lista,Res),
							Lista==Res,!.
aplica_R1_fila(Lista,Res):-
							aplica_R1_fila_aux(Lista,Temp),
							Lista\==Temp,
							aplica_R1_fila(Temp,Res).


% ---------------------------------------------------
% aplica_R2_fila(Fila,N_Fila) aplica a regra 2 a Fila
% ---------------------------------------------------

% --- aux: preenche_0(L,Res) preenche com 0's todas as variaveis
% da lista L---
preenche_0([],[]):-!.
preenche_0([P|R],[0|Res]):-
							var(P),!,
							preenche_0(R,Res).
preenche_0([P|R],[P|Res]):-
							nonvar(P),
							preenche_0(R,Res).

% --- aux: preenche_1(L,Res) preenche com 1's todas as variaveis
% da lista L---
preenche_1([],[]):-!.
preenche_1([P|R],[1|Res]):-
							var(P),!,
							preenche_1(R,Res).
preenche_1([P|R],[P|Res]):-
							nonvar(P),
							preenche_1(R,Res).

aplica_R2_fila([P|R],Res):-
						length([P|R],C),
						N is C//2,
						contador0([P|R],N0),
						N == N0,!,
						preenche_1([P|R],Res).
aplica_R2_fila([P|R],Res):-
						length([P|R],C),
						N is C//2,
						contador1([P|R],N1),
						N == N1,!,
						preenche_0([P|R],Res).
aplica_R2_fila([P|R],Res):-
						length([P|R],C),
						N is C//2,
						contador0([P|R],N0),
						contador1([P|R],N1),
						N>=N0,N>=N1,
						Res = [P|R].

% ---------------------------------------------------
% aplica_R1_R2_fila(Fila,N_Fila) aplica a regra 1 e 
% e depois a regra 2
% ---------------------------------------------------

aplica_R1_R2_fila(Mat_l,Result):-
						aplica_R1_fila(Mat_l,Res),
						aplica_R2_fila(Res,Result).

% --------------------------------------------------
% aplica_R1_R2_puzzle(Puz,N_Puz) aplica o predicado aplica_R1_R2_fila
% as linhas e depois as colunas
% ---------------------------------------------------
aplica_R1_R2_puzzle_l([],[]):-!.
aplica_R1_R2_puzzle_l([P|R],[R1|Res_l]):-
							aplica_R1_R2_fila(P,R1),
							aplica_R1_R2_puzzle_l(R,Res_l).


aplica_R1_R2_puzzle(Mat_l,Res):-
							aplica_R1_R2_puzzle_l(Mat_l,Res_l),
							mat_transposta(Res_l,Trans),
							aplica_R1_R2_puzzle_l(Trans,Res_c),
							mat_transposta(Res_c,Res).


% ---------------------------------------------------
% inicializa(Puz, N_Puz) aplica o predicado aplica_R1_R2_puzzle
% ate nao haver mais alteracoes
% ---------------------------------------------------
inicializa(Puz,Res):-
					aplica_R1_R2_puzzle(Puz,Res),
					Puz==Res,!.
inicializa(Puz,Res):-
					aplica_R1_R2_puzzle(Puz,Temp),
					Puz\==Temp,
					inicializa(Temp,Res).


% ---------------------------------------------------
% verifica_R3(Puz) ve se todas as linhas sao diferentes entre si 
% e o mesmo para as colunas
% ---------------------------------------------------

% --- aux: member_aux(Elemento,Lista) verifica se o Elemento e
% membro da lista (nao unifica como o built in)---
member_aux(E,[P,R]):-
				E\==P,!,
				member_aux(E,R).
member_aux(E,[P|_]):-
				E==P.

% --- aux: verifica_R3_aux(Puz) ve se as linhas do puzzle recebido
% sao todas diferentes---
verifica_R3_aux([]):-!.
verifica_R3_aux([P|R]):-
				not(member_aux(P,R)),
				verifica_R3_aux(R).

% --- aux: busca_cols(Linhas,Colunas) faz uma matriz(Colunas) que e a
% transposta das Linhas---
busca_cols(Mat_l,Mat_c):-
						mat_dimensoes(Mat_l,_,N_col),
						findall(X,(between(1,N_col,C),mat_elementos_coluna(Mat_l,C,X)),Mat_c).

verifica_R3(Mat_l):-
					verifica_R3_aux(Mat_l),
					busca_cols(Mat_l,Mat_c),
					verifica_R3_aux(Mat_c).

% ------------------------------------------------------
% propaga_posicoes(Posicoes,Puz,N_Puz) N_Puz e o resultado
% de propagar 
% ------------------------------------------------------

% --- aux: dif(Lista1,Lista2,Pos) lista Pos com os indices onde 
% ha mudancas entre a Lista1 e a Lista2 ---
dif([P|R],[H|T],Res):-dif([P|R],[H|T],1,Res).
dif([],[],_,[]):-!.
dif([P|R],[H|T],Cont,[Cont|Res]):-
				P\==H,!,
				Cont1 is Cont +1,
				dif(R,T,Cont1,Res).
dif([P|R],[H|T],Cont,Res):-
				P==H,
				Cont1 is Cont +1,
				dif(R,T,Cont1,Res).
				
% --- aux: emparelha_l(Linha,Indices,Coor) faz pares de coordenadas---
emparelha_l(_,[],[]):-!.
emparelha_l(L,[P|R],[(L,P)|Res]):- emparelha_l(L,R,Res).

% --- aux: emparelha_c(Coluna,Indices,Coor) faz pares de coordenadas---
emparelha_c(_,[],[]):-!.
emparelha_c(L,[P|R],[(P,L)|Res]):- emparelha_c(L,R,Res).


propaga_posicoes([],Puz,Puz):-!.
propaga_posicoes([(L,C)|R], Puz,Res):-
						nth1(L,Puz,Lista),
						aplica_R1_R2_fila(Lista,Temp),
						dif(Lista,Temp,Indices_l),
						emparelha_l(L,Indices_l,Coor_l),
						mat_muda_linha(Puz,L,Temp,Puz_temp),
						mat_elementos_coluna(Puz_temp,C,Els),
						aplica_R1_R2_fila(Els,Aux),
						dif(Els,Aux,Indices_c),
						emparelha_c(C,Indices_c,Coor_c),
						mat_muda_coluna(Puz_temp,C,Aux,Puz_temp1),
						append(Coor_l,Coor_c,K),
						append(K,R,Coor),
						propaga_posicoes(Coor,Puz_temp1,Res).


% ----------------------------------------------------------
% resolve(Puz,Sol) encontra uma solucao
% ----------------------------------------------------------

% --- aux: busca_var(Puz,Coor) vai buscar as coordenadas das 
% variaveis existentes em Puz---
busca_var(Puz,Coor):-
					mat_ref(Puz,Coor,V),
					var(V).
% --- aux: resolve_aux(Puz,Lista_vars,Sol) tenta meter um 0 e 
% ve se da para a solucao, se nao, tenta com 1. Isto ate nao
% haver mais variaveis---
resolve_aux(Puz,[],Puz):-!.
resolve_aux(Puz,[P|R],Sol):- 
							mat_muda_posicao(Puz,P,0,N_Puz),
							propaga_posicoes([P],N_Puz,Res),
							verifica_R3(Res),
							resolve_aux(Res,R,Sol).
resolve_aux(Puz,[P|R],Sol):-
							mat_muda_posicao(Puz,P,1,N_Puz),
							propaga_posicoes([P],N_Puz,Res),
							verifica_R3(Res),
							resolve_aux(Res,R,Sol).

resolve(Puz,Sol):-
				inicializa(Puz,N_Puz),
				findall(Coor,busca_var(N_Puz,Coor),Lista_vars),
				resolve_aux(N_Puz,Lista_vars,Sol).