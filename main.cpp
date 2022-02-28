#include <ctime>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>


using namespace std;

string path = "./disk.dk";

struct Partition {
  int size = 0;
  char unit; 
  char name[25]; 
};

struct MBR
{
  int mbr_tamano;
  char mbr_fecha_creacion[17];
  int mbr_dsk_signature;
  Partition partitions[4];
};

/**
 * Formato de fecha
 * @brief Crea una fecha estándar
 * @param  {int} field
 */
string format_day_prop(int field) {
  return (field < 10 ? "0" : "") + to_string(field);
}


/**
 * Obtener fecha como string
 * @brief Retorna un string de la fecha actual
 *
 */
string get_now() {
  time_t time_now = time(0);
  tm *now = localtime(&time_now);
  string current_date =
      format_day_prop(now->tm_mday) + "/" + format_day_prop(now->tm_mon + 1) +
      "/" + to_string(now->tm_year + 1900) + " " +
      format_day_prop(now->tm_hour) + ":" + format_day_prop(now->tm_min);
  return current_date;
}


void write()
{
  // STRUCT QUE VAMOS A GUARDAR
  MBR data;

  // DATOS DE PRUEBA
  int mbr_tamano = 1024 * 1024 * 5; // 1024 === 1 kb * 1024 = 1 mb * 5 = 5mb
  string mbr_fecha_creacion = get_now(); // VER FUNCION ARRIBA
  int mbr_dsk_signature = rand() % 10; // NUMERO RANDOM

  // PARA ESCRIBIR STRINGS SE USA strcpy (SE DEBE ESCRIBIR EN EL ORDEN EN EL QUE SE DEFINIÓ EL STRUCT)
  data.mbr_tamano = mbr_tamano;
  strcpy(data.mbr_fecha_creacion, mbr_fecha_creacion.c_str());
  data.mbr_dsk_signature = mbr_dsk_signature;

  // AQUÍ ABRIMOS COMO LECTURA Y ESCRITURA (rb+) EL ARCHIVO BINARIO
  FILE *disk_file = fopen(path.c_str(), "rb+");

  // FSEEK NOS POSICIONA DENTRO DEL ARCHIVO
  fseek(disk_file, 0, SEEK_SET); // EL SEGUNDO PARAMETRO ES LA POSICIÓN (0 EN ESTE CASO)

  // CREAR UN KB DE 0
  char buffer[1024];
  for (int i = 0; i < 1024; i++)
    buffer[i] = '\0'; // AQUÍ CREAMOS UN KB DE BYTES COMO ARREGLO PARA RELLENAR

   // RELLENAR TODO EL ARCHIVO CON 0
  for (int byte_index = 0; byte_index < mbr_tamano/1024; byte_index++) // VAMOS A LLENAR 1KB A LA VEZ (ASI ES MAS RÁPIDO)
    fwrite(&buffer, 1024, 1, disk_file);

   // FSEEK NOS POSICIONA DENTRO DEL ARCHIVO
  fseek(disk_file, 0, SEEK_SET);

  // CON FWRITE GUARDAMOS EL STRUCT LUEGO DE POSICIONARNOS
  fwrite(&data, sizeof(MBR), 1, disk_file); // EL TERCER PAREMETRO ES LA CANTIDAD DE STRUCTS A GUARDAR (EN ESTE CASO SOLO 1)

  // CERRAR STREAM (importante)
  fclose(disk_file);
}

void fdisk(int size, char unit, string path, string name){
  // LEER MBR
  MBR mbr;
  FILE *disk_file = fopen(path.c_str(), "r+");
  fseek(disk_file, 0, SEEK_SET);
  fread(&mbr, sizeof(MBR), 1, disk_file);

  // BUSCAR PARTICION LIBRE
  int partitionIndex = 0;
  for(int i =0; i < 4; i++) {
    if(mbr.partitions[i].size == 0){
      partitionIndex = i;
      break;
    }
  }

  // AGREGAR PARTICION
  Partition nuevaPart;
  nuevaPart.size = 1024 * (unit == 'm' ? 1024 : 1) * size;
  nuevaPart.unit = unit;
  strcpy(nuevaPart.name, name.c_str());

  mbr.partitions[partitionIndex] = nuevaPart;

  // GUARDAR
  fseek(disk_file, 0, SEEK_SET);
  fwrite(&mbr, sizeof(MBR), 1, disk_file);

  fclose(disk_file);
}

void rep()
{
  MBR rep;

  FILE *disk_file = fopen(path.c_str(), "r+");
  fseek(disk_file, 0, SEEK_SET); 
  fread(&rep, sizeof(MBR), 1, disk_file); 
  fclose(disk_file);

  cout << "--- MBR ---" << endl;
  cout << rep.mbr_tamano << endl
       << rep.mbr_fecha_creacion << endl
       << rep.mbr_dsk_signature << endl;

  cout << "--- PARTICIONES ---" << endl;
  for(int i =0; i < 4;i++) {
    cout << rep.partitions[i].size << endl
         << rep.partitions[i].unit << endl
         << rep.partitions[i].name << endl;
    cout << "------------" << endl;
  }
}

void abrir(string path){
  ifstream archivo(path);
  string linea;
  while (getline(archivo,linea))
  {
    if (linea=="mkdisk")
    {
      write();
    }else if (linea=="fdisk")
    {
      fdisk(3, 'm', "disk.dk", "primero");
      fdisk(1, 'm', "disk.dk", "segundo");
    }
    
    else
    {
      rep();
    }
    
  }
  
}

int main(int argc, char *argv[])
{
  cout<<"BRAYAN HAMLLELO ESTEVEM PRADO MARROQUN"<<endl
      <<"CARNET - 201801369"<<endl;
  abrir("prueba.txt");
  //write();
  //rep();
}
