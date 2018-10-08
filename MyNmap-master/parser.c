#include "parser.h"

double my_power(int x, int y){
    return (y==1) ? x : x*my_power(x,y-1);
}

int number(char *buff){
    char **address_parts= malloc(2*sizeof(char*));
    /* On sépare l'adresse de son netmask */
    address_parts[0]=strtok(buff,"/");
    address_parts[1]=strtok(NULL,"/");
    fflush(stdout);

    int num = (int) my_power(2,32-atoi(address_parts[1])) -2;
    free(address_parts);
    return num;
}

char** parse(char* network){
    char *buff=malloc(sizeof(char)*(strlen(network)+1));
    *buff='\0';
    strcat(buff, network);
    char **address_parts= malloc(2*sizeof(char*));
    /* On sépare l'adresse de son netmask*/
    address_parts[0]=strtok(buff,"/");
    address_parts[1]=strtok(NULL, "/");
    
    int number_of_addresses = (int) my_power(2,32-atoi(address_parts[1])) -2; 
    //pour avoir le nombre d'addresses donc 2**32-longueur_addresse_reseau_local-2 
    // (car adresse locale et addresse de broadcast)

    int *ip_parts= malloc(4*sizeof(int));
    ip_parts[0]=atoi(strtok(address_parts[0],"."));
    ip_parts[1]=atoi(strtok(NULL,"."));
    ip_parts[2]=atoi(strtok(NULL,"."));
    ip_parts[3]=atoi(strtok(NULL,"."));

    char **addresses=malloc(number_of_addresses*sizeof(char*));

    for(int j=0;j<number_of_addresses;j++){
        int part=3;
        if(ip_parts[part]==255){ // si on est à *.*.*.255
            part--; // on remonte pour taper dans la partie suivante de l'adresse
        }
        ip_parts[part]++; //on incrémente genre *.*.*.250 donne *.*.*.251

        char *address_to_stock=malloc(16*sizeof(char)); //on reconstitue une nouvelle adresse
        *address_to_stock='\0';
        char num[3];
        for(int i=0;i<3;i++){
            sprintf(num,"%d",ip_parts[i]);
            address_to_stock=strcat(address_to_stock,num);
            address_to_stock=strcat(address_to_stock,".");
        } 
        sprintf(num,"%d",ip_parts[3]);
        address_to_stock=strcat(address_to_stock,num);

        addresses[j]=address_to_stock;
        //free(address_to_stock);
    }
    // libération
    free(ip_parts);
    free(address_parts);
    free(buff);
    return addresses;
}