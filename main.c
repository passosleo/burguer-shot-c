#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string.h>
#include "utils.h"

//-------------- Constants --------------

#define UP 'A'
#define DOWN 'B'
#define NEXT 'C'
#define BACK 'D'

#define MIN 1
#define MAX 3

#define ADM_PASSWORD 123

//-------------- Structs Declarations --------------

struct product_registry {
  int id;
  char name[50];
  float price;
};

struct order_registry {
  int id;
  char bread[50];
  float bread_price;
  char meat[50];
  float meat_price;
  char cheese[50];
  float cheese_price;
  float total;
};

//-------------- Global Variables --------------

FILE *database;
FILE *database_temp;
int lastId = 0, keyPressed = 0, pos = 1;
short found;

//-------------- Structs --------------

struct product_registry product;
struct order_registry order;
struct order_registry orderRead;

//-------------- Functions Declarations --------------

//Handlers
char getArrow();
int goBack();
int confirm(); 
void clearSelect();
void dot(int realPos, int dotPos);

//Database
void openDatabase(char type, int temp);
void closeDatabase(int temp);

//Menus
int adminMenu();
int userMenu();
int ingredientsMenu(char title[]);
int ingredientSelection(char type, char title[]);

//CRD
void registerIngredient(char type);
void readIngredients(char type);
void deleteIngredient(char type);
void registerOrder();
void readOrders();

//Operations
void generateTicket();
void generateStats();

//Graphics
void header();

//-------------- Functions --------------

//Handlers
char getArrow(){
  int arrow;
  
  if (getch() == '\033') {
    getch();
    arrow = getch();
  };

  return arrow;
};

int goBack(){
  clearSelect();
  
  printf("\n[<] Voltar");
  
  while (keyPressed != BACK) {
    keyPressed = getArrow();
  };

  return 1;
};

int confirm(){
  clearSelect();

  printf("Confirmar pedido?\n");
  printf("\n[<] Cancelar");
  printf("  [>] Confirmar\n");
  
  while (keyPressed != NEXT && keyPressed != BACK) {
    keyPressed = getArrow();
  };

  if(keyPressed == NEXT){
    return 1;
  } else {
    return 0;
  };
};

void clearSelect(){
  keyPressed = 0;
  pos = 1;
};

void dot(int realPos, int dotPos) {
  if (realPos == dotPos) {
    printf("[>] ");
  } else {
    printf("[ ] ");
  };
};

//Database
void openDatabase(char type, int temp) {
  if(!temp){
    switch(type){
      case 'b':
        database = fopen ("./database/bread.bin", "ab+");
        break;
      case 'm':
        database = fopen ("./database/meat.bin", "ab+");
        break;
      case 'c':
        database = fopen ("./database/cheese.bin", "ab+");
        break;
      case 'o':
        database = fopen("./database/orders.bin", "ab+");
    };
  } else {
    switch(type){
      case 'b':
        database_temp = fopen ("./database/bread_temp.bin", "ab+");
        break;
      case 'm':
        database_temp = fopen ("./database/meat_temp.bin", "ab+");
        break;
      case 'c':
        database_temp = fopen ("./database/cheese_temp.bin", "ab+");
        break;
      case 'o':
        database = fopen("./database/orders_temp.bin", "ab+");
    };
  };
  
  if (database == NULL && database_temp == NULL) {
    printf("\n\nErro interno. Falha na conexao com a base de dados.\n\n");
    exit(0);
  };
};

void closeDatabase(int temp){
  if(!temp){
    fclose(database);
    database = NULL;
  } else {
    fclose(database_temp);
    database_temp = NULL;
  };
};

//Menus
int adminMenu() {
  const int max = 5;

  clearSelect();
  
  while (keyPressed != NEXT) {
    clearScreen();
    header();
    printf("Menu\n\n");

    dot(1,pos); printf("Ver ingredientes\n");
    dot(2,pos); printf("Cadastrar ingrediente\n");
    dot(3,pos); printf("Excluir ingrediente\n");
    dot(4,pos); printf("Estatísticas\n");
    dot(5,pos); printf("Sair\n");

    printf("\n[>] Selecionar\n");

    keyPressed = getArrow();

    if (keyPressed == DOWN && pos != max) {
      pos++;
    } else if (keyPressed == UP && pos != MIN) {
      pos--;
    } else {
      pos = pos;
    };
  };

  return pos;
};

int userMenu() {
  clearSelect();
  
  while (keyPressed != NEXT) {
    clearScreen();
    header();
    printf("Menu\n\n");

    dot(1,pos); printf("Montar hamburguer\n");
    dot(2,pos); printf("Meus pedidos\n");
    dot(3,pos); printf("Sair\n");

    printf("\n[>] Selecionar\n");

    keyPressed = getArrow();

    if (keyPressed == DOWN && pos != MAX) {
      pos++;
    } else if (keyPressed == UP && pos != MIN) {
      pos--;
    } else {
      pos = pos;
    };
  };

  return pos;
};

int ingredientsMenu(char title[]){
  clearSelect();
  
  while (keyPressed != NEXT && keyPressed != BACK) {
    clearScreen();
    header();
    printf("%s\n\n",title);

    dot(1,pos); printf("Pão\n");
    dot(2,pos); printf("Carne\n");
    dot(3,pos); printf("Queijo\n");

    printf("\n[<] Voltar");
    printf("  [>] Selecionar\n");
    
    keyPressed = getArrow();

    if (keyPressed == DOWN && pos != MAX) {
      pos++;
    } else if (keyPressed == UP && pos != MIN) {
      pos--;
    } else {
      pos = pos;
    };
  };

  if(keyPressed == NEXT){
    return pos;
  } else {
    return 0;
  };
};

int ingredientSelection(char type, char title[]) {
  int i = 0, max;
  
  clearSelect();
  
  while (keyPressed != NEXT && keyPressed != BACK) {
    clearScreen();
    printf("%s\n\n", title);

    openDatabase(type, 0);

    while(fread(&product, sizeof(struct product_registry), 1, database)) {
      found = 1;
      i++;
      
      dot(i ,pos); printf("%s\n    R$ %.2F\n\n", product.name, product.price);
    };

    if(!found){
      printf("Não há registros\n");
    };
  
    found = 0;
    max = i;
    i = 0;

    printf("\n[<] Voltar");
    printf("  [>] Selecionar\n");
    
    keyPressed = getArrow();

    if (keyPressed == DOWN && pos != max) {
      pos++;
    } else if (keyPressed == UP && pos != MIN) {
      pos--;
    } else {
      pos = pos;
    };

    closeDatabase(0);
  };

  if(keyPressed == NEXT){
    openDatabase(type, 0);
    
    while(fread(&product, sizeof(struct product_registry), 1, database) && i != pos - 1) {
      i++;
    };

    switch(type){
      case 'b':
        strcpy(order.bread, product.name);
        order.bread_price = product.price;
        break;
      case 'm':
        strcpy(order.meat, product.name);
        order.meat_price = product.price;
        break;
      case 'c':
        strcpy(order.cheese, product.name);
        order.cheese_price = product.price;
    };
    
    closeDatabase(0);
    return 1;
  } else {
    return 0;
  };
};

//CRD
void registerIngredient(char type){
  clearScreen();
  openDatabase(type, 0);

  while(fread(&product, sizeof(struct product_registry), 1, database)) {
    lastId = product.id;
  };
  
  printf("\nID: %d",product.id = lastId + 1);
  
  printf("\nNome: ");
  fgets(product.name, 50, stdin);
  product.name[strlen(product.name) - 1] = '\0';

  printf("\nValor: R$ ");
  scanf("%f%*c",&product.price);

  fwrite (&product, sizeof(struct product_registry), 1, database);

  lastId = 0;
  
  closeDatabase(0);
};

void readIngredients(char type){
  clearScreen();
  openDatabase(type, 0);

  while(fread(&product, sizeof(struct product_registry), 1, database)) {
    found = 1;
    printf ("\nID: %d\nNome: %s\nValor: R$%.2f\n\n", product.id, product.name, product.price);
  };

  if(!found){
    printf("Não há registros\n");
  };
  
  found = 0;

  closeDatabase(0);
};

void deleteIngredient(char type){
  int id = 0;
  
  clearScreen();
  openDatabase(type, 0);
  openDatabase(type, 1);

  printf("Informe o ID: ");
  scanf("%d%*c",&id);

  clearScreen();
  
  while (fread(&product,sizeof(struct product_registry),1,database)) {
		if (product.id == id) {
			printf("%s deletado com sucesso\n\n",product.name);
			found = 1;
		} else {
			fwrite(&product, sizeof(struct product_registry), 1, database_temp);
		};
	};

  if(!found){
    printf("ID %d não encontrado\n",id);
  }
  
  found = 0;

  closeDatabase(0);
  closeDatabase(1);

  switch(type){
    case 'b':
      remove("./database/bread.bin");
	    rename("./database/bread_temp.bin", "./database/bread.bin");
      break;
    case 'm':
      remove("./database/meat.bin");
	    rename("./database/meat_temp.bin", "./database/meat.bin");
      break;
    case 'c':
      remove("./database/cheese.bin");
	    rename("./database/cheese_temp.bin", "./database/cheese.bin");
      break;
  };

  goBack();
};

void registerOrder(){
  openDatabase('o', 0);
          
  while(fread(&orderRead, sizeof(struct order_registry), 1, database)) {
    lastId = orderRead.id;
  };

  order.id = lastId + 1;

  fwrite (&order, sizeof(struct order_registry), 1, database);

  lastId = 0;
  
  closeDatabase(0);
};

void readOrders(){
  clearScreen();
  openDatabase('o', 0);

  printf("============================\n");
  while(fread(&order, sizeof(struct order_registry), 1, database)) {
    found = 1;
    
    printf ("\nPedido n°: %d\n\nPão: %s - R$ %.2f\nCarne: %s - R$ %.2f\nQuejo: %s - R$ %.2f\n\nTotal: R$ %.2f\n\n", order.id, order.bread, order.bread_price, order.meat, order.meat_price, order.cheese, order.cheese_price, order.total);
    printf("============================\n");
  };
  
  if(!found){
    printf("Não há registros\n");
  };
  
  found = 0;
  
  closeDatabase(0);
  goBack();
};

//Operations
void generateTicket(){
  char buffer[50];
  
  database = fopen("order.txt", "w");

  if (database == NULL) {
    printf("\n\nErro interno. Falha na conexao com a base de dados.\n\n");
    exit(0);
  };

  fputs("===========================\n", database);
  fputs("Pedido:\n\n", database);
  fputs("Pão ", database);
  fputs(order.bread, database);
  fputs("\n", database);
  fputs("Carne ", database);
  fputs(order.meat, database);
  fputs("\n", database);
  fputs("Quejo ", database);
  fputs(order.cheese, database);
  fputs("\n\n", database);
  fputs("Total R$", database);
  gcvt(order.total, 4, buffer);
  fputs(buffer, database);
  fputs(",00", database);
  fputs("\n===========================\n", database);

  clearScreen();
  
  printf("Ticket gerado com sucesso em order.txt\n\n");
  
  closeDatabase(0);
};

void generateStats(){
  int counter = 0;
  float totalPrice = 0, highestPrice = 0, lowestPrice = 0;
  
  clearScreen();
  printf("Estatísticas:\n\n");
  
  openDatabase('b', 0);
  
  while(fread(&product, sizeof(struct product_registry), 1, database)) {   
    found = 1;
    counter++;
    totalPrice = totalPrice + product.price;
  
    if(product.price > highestPrice){
      highestPrice = product.price;
    };
  
    if(product.price < lowestPrice || lowestPrice == 0){
      lowestPrice = product.price;
    };
  };
  
  if(!found){
    printf("Não há registros de pães\n\n");
  } else {
    printf("--------Ingredientes--------\n\n");
    printf("Variedade de pães: %d\n", counter);
    printf("Valor pão mais caro: R$ %.2f\n", highestPrice);
    printf("Valor pão mais barato: R$ %.2f\n", lowestPrice);
    printf("Valor medio do pão: R$ %.2f\n\n", totalPrice/counter);
  };
  
  found = 0;
  counter = 0;
  totalPrice = 0;
  highestPrice = 0;
  lowestPrice = 0;
  
  closeDatabase(0);
  
  openDatabase('m', 0);
  
  while(fread(&product, sizeof(struct product_registry), 1, database)) {   
    found = 1;
    counter++;
    totalPrice = totalPrice + product.price;
  
    if(product.price > highestPrice){
      highestPrice = product.price;
    };
  
    if(product.price < lowestPrice || lowestPrice == 0){
      lowestPrice = product.price;
    };
  };
  
  if(!found){
    printf("Não há registros de carnes\n\n");
  } else {
    printf("Variedade de carnes: %d\n", counter);
    printf("Valor carne mais cara: R$ %.2f\n", highestPrice);
    printf("Valor carne mais barata: R$ %.2f\n", lowestPrice);
    printf("Valor medio da carne: R$ %.2f\n\n", totalPrice/counter);
  };
  
  found = 0;
  counter = 0;
  totalPrice = 0;
  highestPrice = 0;
  lowestPrice = 0;
  
  closeDatabase(0);
  
  openDatabase('c', 0);
  
  while(fread(&product, sizeof(struct product_registry), 1, database)) {   
    found = 1;
    counter++;
    totalPrice = totalPrice + product.price;
  
    if(product.price > highestPrice){
      highestPrice = product.price;
    };
  
    if(product.price < lowestPrice || lowestPrice == 0){
      lowestPrice = product.price;
    };
  };
  
  if(!found){
    printf("Não há registros de queijos\n\n");
  } else {
    printf("Variedade de queijos: %d\n", counter);
    printf("Valor carne mais cara: R$ %.2f\n", highestPrice);
    printf("Valor carne mais barata: R$ %.2f\n", lowestPrice);
    printf("Valor medio do queijo: R$ %.2f\n\n", totalPrice/counter);
  };

  found = 0;
  counter = 0;
  totalPrice = 0;
  highestPrice = 0;
  lowestPrice = 0;
  
  closeDatabase(0);
  
  openDatabase('o', 0);
  
  while(fread(&order, sizeof(struct order_registry), 1, database)) {   
    found = 1;
    totalPrice = totalPrice + order.total;
  
    if(order.total > highestPrice){
      highestPrice = order.total;
    };
  
    if(order.total < lowestPrice || lowestPrice == 0){
      lowestPrice = order.total;
    };
  };
  
  if(!found){
    printf("Não há registros de pedidos\n\n");
  } else {
    printf("----------Pedidos-----------\n\n");
    printf("Total de pedidos: %d\n", order.id);
    printf("Valor medio do hamburguer: R$ %.2f\n", totalPrice/order.id);
    printf("Valor hamburguer mais caro: R$ %.2f\n", highestPrice);
    printf("Valor hamburguer mais barato: R$ %.2f\n\n", lowestPrice);
  };
  
  found = 0;
  counter = 0;
  totalPrice = 0;
  highestPrice = 0;
  lowestPrice = 0;
  
  closeDatabase(0);
};

//Graphics
void header(){
  printf("      *     .-''''-.       *\n");
  printf("           /' .  '. \\  *\n");
  printf("   *      (`-..:...-')         *\n");
  printf("       *   ;-......-;     *\n");
  printf("  *         '------'         *\n");
  printf("===================================\n");
  printf("╔╗ ┬ ┬┬─┐┌─┐┬ ┬┌─┐┬─┐  ╔═╗┬ ┬┌─┐┌┬┐\n");
  printf("╠╩╗│ │├┬┘│ ┬│ │├┤ ├┬┘  ╚═╗├─┤│ │ │\n");
  printf("╚═╝└─┘┴└─└─┘└─┘└─┘┴└─  ╚═╝┴ ┴└─┘ ┴\n");
  printf("===================================\n\n");
};


//-------------- Main Function --------------

int main(int argc, char **argv) {
  short isValid;
  int password, attempts = 3;
  float total;

  clearScreen();
  
  if(argv[1] == NULL){
    printf("Argumentos:\n\n-user = Ambiente do usuário\n-admin = Ambiente administrativo\n-sobre = Informações do software\n\n");
    exit(0);
  };

  if(!strcmp(argv[1], "-admin")){
    do{
      printf("Informe a senha: ");
      scanf("%d%*c",&password);
      isValid = 1;

      if(password != ADM_PASSWORD){
        clearScreen();

        if(attempts == 0){
          exit(0);
        };
        
        printf("Senha inválida!\n");
        printf("%d tentativas restantes\n\n", attempts);
        
        isValid = 0;
        attempts--;
      };
    } while(!isValid);
    
    while(1){
      switch(adminMenu()){
        case 1:
          INGREDIENT_SELECT:
          switch(ingredientsMenu("Ver ingredientes")){
            case 1:
              readIngredients('b');
              
              if(goBack()){
                goto INGREDIENT_SELECT;
              };
              
              break;
            case 2: 
              readIngredients('m');
                            
              if(goBack()){
                goto INGREDIENT_SELECT;
              };
              
              break;
            case 3:
              readIngredients('c');
                            
              if(goBack()){
                goto INGREDIENT_SELECT;
              };
              
              break;
          };
          break;
        case 2:
          switch(ingredientsMenu("Cadastrar ingrediente")){
            case 1:
              registerIngredient('b');
              break;
            case 2: 
              registerIngredient('m');
              break;
            case 3:
              registerIngredient('c');
              break;
            };
          break;
        case 3:
          switch(ingredientsMenu("Excluir ingrediente")){
            case 1:
              deleteIngredient('b');
              break;
            case 2: 
              deleteIngredient('m');
              break;
            case 3:
              deleteIngredient('c');
              break;
          };
          break;
        case 4:
          generateStats();
          goBack();
          break;
        case 5:
          clearScreen();
          exit(0);
      };
    };
  };
  
  if(!strcmp(argv[1], "-user")) {
    while(1){
      switch(userMenu()) {
        case 1:
          BREAD_SELECT:
          if(!ingredientSelection('b', "Escolha o pão")){
            break;
          };
  
          MEAT_SELECT:
          if(!ingredientSelection('m', "Escolha a carne")){
            goto BREAD_SELECT;
          };
  
          CHEESE_SELECT:
          if(!ingredientSelection('c', "Escolha o queijo")){
            goto MEAT_SELECT;
          };

          clearScreen();

          order.total = order.meat_price + order.bread_price + order.cheese_price;
          printf("============================\n\n");
          printf("Resumo do pedido:\n\n");
          printf("Pão:\n%s - R$ %.2f\n\nCarne:\n%s - R$ %.2f\n\nQueijo:\n%s - R$ %.2f\n\n", order.bread, order.bread_price, order.meat, order.meat_price, order.cheese, order.cheese_price);
          printf("\nValor total: R$ %.2f\n\n", order.total);
          printf("============================\n\n");

          if(!confirm()) {
            break;
          };

          generateTicket();
          registerOrder();
          goBack();
          break;
        
        case 2:
          readOrders();
          break;
        
        case 3:
          clearScreen();
          exit(0);
      };
    };
  };

  if(!strcmp(argv[1], "-sobre")) {
    clearScreen();
    printf("Software: Burger Shot\n");
    printf("Versão: 1.0\n");
    printf("Desenvolvedor: Leonardo Passos\n");
    printf("Disciplina: Programação II\n");
    printf("Professor: Leonardo Sápiras\n");
    printf("Semestre: 2022/1\n");
    printf("Instituição: FACCAT\n\n");
    exit(0);
  };

  printf("Argumento inválido\n\n");
  
  return 0;
};
