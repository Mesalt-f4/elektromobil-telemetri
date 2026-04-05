#ifndef TELEMETRY_H
#define TELEMETRY_H

#define MAX_KAYIT 100
#define MAX_HIZ 70.0f
#define ORTAM_SICAKLIGI 20.0f
#define KRITIK_MOTOR_SICAKLIGI 90.0f
#define KRITIK_BATARYA_SICAKLIGI 70.0f

void sistemi_kur(float baslangic_sicaklik, int baslangic_sarj);
void gaza_bas();
void frene_bas();
void rejen_fren();
void telemetri_ve_istatistik_yazdir();
void sistemi_kapat();

#endif