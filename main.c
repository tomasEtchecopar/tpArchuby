#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define cambioUSD 244


//dos structs de usuarios. una para contraseñas y otro para las cuentas. estan relacionaods mediante arreglos paralelos de structs.
typedef struct{
    char nombreUsuario[30];
    char password[30];

}usuarioContra;

typedef struct{
    char nombreYApellido[50];
    float saldo;
    float saldoUSD;

}cuentaUsuario;

///PROTOTIPADO///


//funciones de menus//
void menus();
void menuUsuario();
int elegirCuenta();
int elegirUsuaroAdmin();

//creado de cuentas//
void crearCuenta();
int buscarIgual(usuarioContra a);
usuarioContra PassNuevo();
cuentaUsuario cuentaNueva();

//iniciar sesion//
void iniciarSesion();
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
            i = tamanioArchi/sizeof(usuarioContra);
        }
        fclose(arch);
    }
    return i;
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

void menus(){
    system("cls");
    int user=elegirUsuaroAdmin();
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

void menuUsuario(){
    int a=elegirCuenta();
    switch(a){
case 1:
    //codigo para iniciar sesion
    break;
case 2:
    system("cls");
    crearCuenta();
    break;
    }
}



int elegirCuenta(){
    int seleccion=1, tecla=0;
    while(tecla!=13){ //TECLA DISTINTA DE ENTER
    system("cls");
    printf("------------BIENVENIDO-------------\n\n");
    flecha(1, seleccion);
    printf("\tINICIAR SESION\t\t");
    flecha(2, seleccion);
    printf("CREAR CUENTA");
    tecla=getch();
    switch(tecla){
    case 77:
        if(seleccion<2){
            seleccion++;
        }
        break;
    case 75:
        if(seleccion>1){
            seleccion--;
        }
        break;
    }
    }
    return seleccion;
}

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
    // 77 DERECHA
    // 75 IZQUIERDA
    switch(tecla){
    case 77:
        if(seleccion<3){
            seleccion++;
        }
        break;
    case 75:
        if(seleccion>1){
            seleccion--;
        }
        break;
    }

}
    return seleccion;
}

///CREADO DE CUENTAS///

void crearCuenta(){
    FILE *user=fopen("passwords.dat", "ab");
    FILE *cuentas=fopen("cuentas.dat", "ab");
    usuarioContra a;
    cuentaUsuario acc;
    if(user!=NULL && cuentas!=NULL){
        int duplicado=1;
        while(duplicado==1){
            a=PassNuevo();
            duplicado=buscarIgual(a);
            if(duplicado==1){
                system("cls");
                printf("Nombre de usuario en uso. Intente de nuevo");
            }
        }
        acc=cuentaNueva();
        fwrite(&a, sizeof(usuarioContra), 1, user);
        fwrite(&acc, sizeof(cuentaUsuario), 1, cuentas);
        fclose(user);
        fclose(cuentas);

    }
}

int buscarIgual(usuarioContra a){ //funcion aux para buscar si un nombre de usuario ya esta en uso
    FILE *arch=fopen("passwords.dat", "rb");
    usuarioContra b;
    int flag=0;
    if(arch!=NULL){
        while(fread(&b, sizeof(usuarioContra), 1, arch)){
            if(strcmp(a.nombreUsuario, b.nombreUsuario)==0){
                flag=1;
            }
        }
        fclose(arch);
    }
    return flag;
}

usuarioContra PassNuevo(){
    usuarioContra a;
    fflush(stdin);
    printf("Ingrese nombre de usuario (30 caracteres max.): ");
    scanf("%29s", a.nombreUsuario);
    fflush(stdin);
    printf("Ingrese password (30 caracteres max.): ");
    scanf("%29s", a.password);
    return a;
}

cuentaUsuario cuentaNueva(){
    cuentaUsuario a;
    fflush(stdin);
    printf("Ingrese nombre y apellido: ");
    gets(a.nombreYApellido);
    printf("Ingrese saldo en pesos: ");
    scanf("%f", &a.saldo);
    a.saldoUSD = a.saldo*cambioUSD;
    return a;
}


///INICIAR SESION


void iniciarSesion(){
    FILE *user=fopen("passwords.dat", "rb");
    FILE *cuentas=fopen("cuentas.dat", "rb");
    char nombreNuevo[30], pass[30];
    int numUsuario=cantDePasswords();
    usuarioContra passCheck;
    cuentaUsuario datos;

    if(user!=NULL && cuentas!=NULL){
        fflush(stdin);
        printf("Nombre de usuario: ");
        scanf("%s", nombreNuevo);
        fflush(stdin);
        printf("Password: ");
        scanf("%s", pass);
        while(fread(&passCheck, sizeof(usuarioContra), 1, user) == 1 && strcmp(nombreNuevo, passCheck.nombreUsuario)!=0){ //
            numUsuario--;
        }

    }
}
