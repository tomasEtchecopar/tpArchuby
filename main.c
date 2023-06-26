#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define cambioUSD 244


//dos structs de usuarios. una para contraseñas y otro para las cuentas. estan relacionaods mediante arreglos paralelos de structs.
typedef struct{
    char nombreUsuario[30];
    int CBU[15];
    char password[30];

}nombrePassword;

typedef struct{
    char nombreYApellido[50];
    int CBU[15];
    char alias[15];
    float saldo;
    float saldoUSD;

}datosUsuarios;

///PROTOTIPADO///

//funcion auxiliares
void ingresarDato(char a[], char b[]);
int buscarUsuario(FILE *user, char a[]);
void arrayAToB(int a[], int b[], int validos);

//funciones de menus//
void menus();
void menuUsuario();
int elegirCuenta();
int elegirUsuaroAdmin();

//creado de cuentas//
void crearCuenta();
int buscarIgual(nombrePassword a);
nombrePassword PassNuevo();
datosUsuarios cuentaNueva();

//iniciar sesion//
void iniciarSesion();
datosUsuarios datosPorNumUsuario(FILE *cuentas, int numUsuario);
int ingresarNombrePassword();
/////////////////

///MAIN///
int main()
{
    menus();

    return 0;
}

//////////////////////////////////////////

int cantDePasswords(){
    FILE *arch=fopen("passwords.dat", "rb");
    int i=-1;
    if(arch!=NULL){
        fseek(arch, 0, SEEK_END);
        int tamanioArchi=ftell(arch);
        if(tamanioArchi!=-1){
            i = tamanioArchi/sizeof(nombrePassword);
        }
        fclose(arch);
    }
    return i;
}

///FUNCIONES AUX///
//usadas a lo largo de todas las funciones

void ingresarDato(char a[], char b[]){//1er parametro texto, 2do input del usuario
    printf("%s", a);
    fflush(stdin);
    gets(b);
}

void arrayAToB(int a[], int b[], int validos){
    for(int i=0;i<validos;i++){
        b[i]=a[i];
    }
}


///FUNCIONES MENU///

//FUNCION PARA LA FLECHA. 1er parametro es la posicion en la que esta el texto, 2do es la posicion que elige el usuario mediante las flechas
void flecha(int posicTexto, int seleccion){
    if(posicTexto==seleccion){
        printf("---->");
    }
    else{
        printf("     ");
    }
}

int posicionDelUsuario(tecla, seleccion){//modularizacion para optimizar la utilizacion de los botones. los menus deben tener tres columnas de botones.
    // 75 IZQUIERDA
    // 77 DERECHA
    // 72 ARRIBA
    // 80 ABAJO
    switch(tecla){
    case 77:
            seleccion++;
        break;
    case 75:
            seleccion--;
        break;
    case 72:
        seleccion-=3;
        break;
    case 80:
        seleccion+=3;
        break;
    }
    return seleccion;


}

///FUNCION VISUAL DEL MENU; RETORNA BOTON PRESIONADO
int elegirUsuaroAdmin(){
    int seleccion=1, tecla=0;

    while(tecla!=13){
    system("cls");

    printf("------------BIENVENIDO-------------\n");
    printf("\tSELECCIONE TIPO DE CUENTA\n\n");
    flecha(1, seleccion); printf("USUARIO\t");
    flecha(2, seleccion); printf("ADMIN\t");
    flecha(3, seleccion); printf("SALIR");
    printf("\n");

    printf("\n");

    tecla = getch();

    if(tecla==13){//retorno seleccion antes de que cambie a 13
        return seleccion;
    }


    seleccion=posicionDelUsuario(tecla, seleccion);

    if(seleccion>3){ //no se me ocurre otra manera de no exceder la cant de botones xd
        seleccion=3;
    }else{
        if(seleccion<1){
            seleccion=1;
        }
    }

}

}

void menus(){
    int user=0;
    while(user!=3){

    system("cls");
    user=elegirUsuaroAdmin();

    switch(user){
    case 1:
        menuUsuario();
        break;
    case 2:
        //menu admin
        break;
    case 3:
        system("cls");
        break;

    }
    }
}

void menuUsuario()  {
    int a=0;
    while(a!=3){
    a=elegirCuenta();
    switch(a){
case 1:
    iniciarSesion();
    break;
case 2:
    system("cls");
    crearCuenta();
    break;
case 3:
    return 0;//para que la ejecucion de la funcion termine y se haga el break de la linea 150, entonces se repite el bucle de 142 pq user==1 al entrar a este menu.
    break;
    }
    }
}



///FUNCION VISUAL DEL MENU; RETORNA BOTON PRESIONADO
int elegirCuenta(){
    int seleccion=1, tecla=0;
    while(tecla!=13){ //TECLA DISTINTA DE ENTER
    system("cls");
    printf("------------BIENVENIDO-------------\n\n");
    flecha(1, seleccion);
    printf("INICIAR SESION\t");
    flecha(2, seleccion);
    printf("CREAR CUENTA\t");
    flecha(3, seleccion);
    printf("VOLVER");

    tecla=getch();

    if(tecla==13){
        return seleccion;
    }

    seleccion=posicionDelUsuario(tecla, seleccion);

    if(seleccion>3){
        seleccion=3;
    }
    else{
        if(seleccion<1){
            seleccion=1;
        }
    }

    }
}




///CREADO DE CUENTAS///

void crearCuenta(){
    FILE *user=fopen("passwords.dat", "ab");
    FILE *cuentas=fopen("cuentas.dat", "ab");
    nombrePassword a;
    datosUsuarios acc;
    if(user!=NULL && cuentas!=NULL){
        int duplicado=1;
        while(duplicado==1){
            printf("------------CREAR CUENTA-------------\n");
            a=PassNuevo();
            duplicado=buscarIgual(a);
            if(duplicado==1){
                system("cls");
                printf("Nombre de usuario en uso. Intente de nuevo");
            }
        }
        acc=cuentaNueva(a.CBU);
        fwrite(&a, sizeof(nombrePassword), 1, user);
        fwrite(&acc, sizeof(datosUsuarios), 1, cuentas);
        fclose(user);
        fclose(cuentas);

    }
}

int buscarIgual(nombrePassword a){ //funcion aux para buscar si un nombre de usuario ya esta en uso
    FILE *arch=fopen("passwords.dat", "rb");
    nombrePassword b;
    int flag=0;
    if(arch!=NULL){
        while(fread(&b, sizeof(nombrePassword), 1, arch)){
            if(strcmp(a.nombreUsuario, b.nombreUsuario)==0){
                flag=1;
            }
        }
        fclose(arch);
    }
    return flag;
}

nombrePassword PassNuevo(){
    nombrePassword a;
    ingresarDato("Ingrese nombre de usuario (30 caracteres max.): ", a.nombreUsuario);
    printf("CBU (15 max.): ");
    scanf("%d", a.CBU);
    ingresarDato("Ingrese password (30 caracteres max.): ", a.password);
    return a;
}

datosUsuarios cuentaNueva(int cbum[]){
    datosUsuarios a;
    fflush(stdin);
    printf("Ingrese nombre y apellido: ");
    gets(a.nombreYApellido);
    arrayAToB(a.CBU, cbum, 15);
    printf("Ingrese saldo en pesos: ");
    scanf("%f", &a.saldo);
    a.saldoUSD = a.saldo*cambioUSD;
    return a;
}


///INICIAR SESION

void iniciarSesion(){
    FILE *cuentas=fopen("cuentas.dat", "rb");
    datosUsuarios userData;
    int numUsuario;
    if(cuentas==NULL){
        return;
    }

    numUsuario=ingresarNombrePassword();

    if(numUsuario==-1){
        return;
    }
    userData=datosPorNumUsuario(cuentas, numUsuario);

    //hacer menu de usuario(ingresar dinero, hacer transferencia, etc.)
}

datosUsuarios datosPorNumUsuario(FILE *cuentas, int numUsuario){
    datosUsuarios datos;

    fseek(cuentas, sizeof(datosUsuarios)*numUsuario, SEEK_SET);
    fread(&datos, sizeof(datosUsuarios), 1, cuentas);
    fclose(cuentas);

    return datos;
}

int ingresarNombrePassword(){
    FILE *user=fopen("passwords.dat", "rb");
    char nombreNuevo[30], passNuevo[30];
    int flagPassword=0, numUsuario=-1, input=0; //input es para volver hacia atras en el menu
    nombrePassword passCheck;
    datosUsuarios datos;

    if(user==NULL){
        fclose(user);

        return 0;
    }
     while(flagPassword==0){
        system("cls");
        printf("------------INICIAR SESION-------------\n");
        fflush(stdin);
        ingresarDato("Nombre de Usuario: ", nombreNuevo);
        ingresarDato("Password: ", passNuevo);

        numUsuario=buscarUsuario(user, nombreNuevo);

        if(numUsuario==-1){
            printf("\nNombre de usuario incorrecto.\n");
            printf("Ingrese cualquier tecla para reintentar. (ESC para salir)\n");
            input=getch();
        }
        else{
            fseek(user, sizeof(nombrePassword)*numUsuario, SEEK_SET);
            fread(&passCheck, sizeof(nombrePassword), 1, user);
            if(strcmp(passNuevo, passCheck.password)!=0){
                printf("\nContrasenia incorrecta.\n");
                printf("Ingrese cualquier tecla para reintentar. (ESC para salir)\n");
                input=getch();

            }
            else{
                flagPassword=1;
            }
        }
        if(input==27){
            return -1;
        }
    }
        fclose(user);

    return numUsuario;
}

int buscarUsuario(FILE *user, char a[]){
    int numUsuario, limite=cantDePasswords();
    nombrePassword passCheck;
    for(numUsuario=0;numUsuario<limite;numUsuario++){
        if(fread(&passCheck, sizeof(nombrePassword), 1, user)==1){
            if(strcmp(a, passCheck.nombreUsuario)==0){
                return numUsuario;
            }
        }
    }
    return -1;
}

