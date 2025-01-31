/**
**	Sigla: stk
**	Lib responsavel por gerenciar a pilha de cards
**
*/

#include <stdlib.h>
#include "standard_deck.h"

//	Struct que representa cada elemento da pilha.
//	Dentro de cada um destes elementos Item, havera
//	uma carta.
struct Item {
	struct Card card;
	struct Item *next;
};

struct Stack {
	struct Item *top;
	struct Item *bottom;
	//	TODO: Verificar se ajuda ou atrapalha usar um int cards_remaining para guardar o tamanho a pilha
};

/**
**    Cria uma nova struct do tipo Stack e a retorna com os ponteiros inicializados
**
**    @return struct Stack: uma variavel do tipo struct Stack pronta para uso
**
**    @author Rafael Chinaglia <chinaglia.rafa@gmail.com>
*/
struct Stack stk_make_stack() {
	struct Stack new_stack;
	new_stack.top = NULL;
	new_stack.bottom = NULL;

	return new_stack;
}

/**
**    Adiciona um card a pilha de cards
**    @param struct Card: o card a ser empilhado
**    @param struct* Stack: ponteiro para a pilha onde o card devera ser inserido
**
**    @author Rafael Chinaglia <chinaglia.rafa@gmail.com>
*/
void stk_push(struct Card card, struct Stack* stack) {
	if(stack == NULL || !crd_is_valid(card)) {
		printf("quebrado");
		return;
	}


	//	Cria um novo espaco na memoria para o novo Item
	struct Item* new_item = (struct Item*)malloc(sizeof(struct Item));
	if (new_item == NULL)
		return;

	new_item->card = card;

	//	Verifica se a pilha esta vazia
	if (stack->top == NULL) {
		stack->top = new_item;
		stack->bottom = new_item;
		new_item->next = NULL;
	} else {
		new_item->next = stack->top;
		stack->top = new_item;
	}
}

/**
**    Remove o card do topo da pilha
**    @param struct* Stack: ponteiro para a pilha de onde o card sera removido
**
** 	  @return struct Card: card retirado
**
**    @author Rafael Chinaglia <chinaglia.rafa@gmail.com>
*/
struct Card stk_pop(struct Stack* stack) {
	if (stack == NULL || stack->top == NULL)
		exit(0);

	//	Faz uma copia do card que esta no topo
	struct Card card_on_top = (stack->top)->card;
	//	Faz uma copia do endereco do topo (para dar free())
	struct Item* item_to_be_freed = stack->top;

	//	Caso bottom e top apontem para o mesmo Item, a pilha tera apenas
	//	um elemento, e depois de removido, bottom deverá ser NULL
	if (stack->bottom == stack->top)
		stack->bottom = NULL;

	stack->top = (stack->top)->next;

	free(item_to_be_freed);

	return card_on_top;
}

/**
**    Verifica se a pilha esta vazia
**    @param struct Stack: pilha a ser analisada
**
** 	  @return short: Sim ou nao
**
**    @author Rafael Chinaglia <chinaglia.rafa@gmail.com>
*/
short stk_is_empty(struct Stack stack) {
	return stack.top == NULL || stack.bottom == NULL;
}

/**
**    Empilha todo o baralho conhecido usando standard_deck.h
**	  provavelmente em ordem.
**    @param struct Stack*: ponteiro para a pilha
**
**    @author Rafael Chinaglia <chinaglia.rafa@gmail.com>
*/
void stk_stack_deck(struct Stack* stack) {
	int i = 0;
	stddk_load_standard_deck();
	for (i = 0; i < STANDARD_DECK.deck_size; i++) {
		stk_push(STANDARD_DECK.cards[i], stack);
	}
}

/**
**    Conta o numero de elementos em uma pilha
**    @param struct Stack*: ponteiro para a pilha
**
**		@return int: numero de elementos na pilha
**
**    @author Rafael Chinaglia <chinaglia.rafa@gmail.com>
*/
int stk_count(struct Stack* stack) {
	int i = 0;
	struct Item* seeker = stack->top;
	while (seeker != NULL) {
		i++;
		seeker = seeker->next;
	}
	return i;
}

/**
**    Embaralha uma stack em uma nova ordem aleatoria
**    @param struct Stack*: ponteiro para a pilha a ser embaralhada
**
**    @author Rafael Chinaglia <chinaglia.rafa@gmail.com>
**		TODO: dinamizar o deck. Por enquanto standar_deck esta fixo
*/
void stk_shuffle(struct Stack* stack) {
	srand(time(NULL));
	*stack = stk_make_stack();
	//	Carrega a pilha de cartas, que sera usada como LISTA
	struct Stack deck_as_list = stk_make_stack();
	stk_stack_deck(&deck_as_list);
	int cards_remaining = stk_count(&deck_as_list);
	int chosen_card = 0, i = 0;
	//	Repete o processo de empilhamento enquanto houverem cards no deck
	while (cards_remaining > 0) {
		if (cards_remaining > 0)
			chosen_card = rand() % cards_remaining;
		else
			 chosen_card = 0;
		struct Item *seeker = deck_as_list.top;
		struct Item *previous = NULL;
		for (i = 0; i < chosen_card; i++) {
			previous = seeker;
			seeker = seeker->next;
		}
		stk_push(seeker->card, stack);
		if (seeker == deck_as_list.top) {
			deck_as_list.top = (deck_as_list.top)->next;
		}	else if (seeker->next == NULL) {
			if (previous != NULL)
				previous->next = NULL;
		}	else {
			if (previous != NULL)
				previous->next = seeker->next;
		}
		cards_remaining--;
	}
}
