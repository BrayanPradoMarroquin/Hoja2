#include <ctime>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>


using namespace std;

string path = "./disk.dk";
struct MBR
{
  int mbr_tamano;
  char mbr_fecha_creacion[17];
  int mbr_dsk_signature;
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

void rep()
{
  // STRUCT QUE VAMOS A LEER
  MBR data;

  // AQUÍ ABRIMOS COMO LECTURA (r+) EL ARCHIVO BINARIO
  FILE *disk_file = fopen(path.c_str(), "r+");

  // FSEEK NOS POSICIONA DENTRO  DEL ARCHIVO
  fseek(disk_file, 0, SEEK_SET); // EL SEGUNDO PARAMETRO ES LA POSICIÓN (0 EN ESTE CASO)

  // CON FREAD LEEMOS EL STRUCT LUEGO DE POSICIONARNOS
  fread(&data, sizeof(MBR), 1, disk_file); // EL TERCER PAREMETRO ES LA CANTIDAD DE STRUCTS A GUARDAR (EN ESTE CASO SOLO 1)

  // CERRAR STREAM (importante)
  fclose(disk_file);

  // AQUÍ MOSTRAMOS EL STRUCT
  cout << data.mbr_tamano << endl
       << data.mbr_fecha_creacion << endl
       << data.mbr_dsk_signature << endl;
}

void abrir(string path){
  ifstream archivo(path);
  string linea;
  while (getline(archivo,linea))
  {
    if (linea=="mkdisk")
    {
      write();
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
  abrir(argv[3]);
  //write();
  //rep();
}
