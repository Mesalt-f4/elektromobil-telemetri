#include "telemetry.h"
#include <stdio.h>
#include <stdlib.h>

static float hiz = 0.0f;
static int batarya = 0;
static float motor_sicakligi = 0.0f;
static float batarya_sicakligi = 0.0f;

static float hizlanma_kayitlari[MAX_KAYIT];
static float yavaslama_kayitlari[MAX_KAYIT];
static float rejen_kayitlari[MAX_KAYIT];

static int hizlanma_sayac = 0;
static int yavaslama_sayac = 0;
static int rejen_sayac = 0;

void sistemi_kur(float baslangic_sicaklik, int baslangic_sarj) {
    hiz = 0.0f;
    batarya = baslangic_sarj;
    motor_sicakligi = baslangic_sicaklik;
    batarya_sicakligi = baslangic_sicaklik;
}

void telemetri_ve_istatistik_yazdir() {
    printf("\n--- ANLIK TELEMETRI ---\n");
    printf("Guncel Hiz: %.1f km/s\n", hiz);
    printf("Batarya: %%%d\n", batarya);
    printf("Motor Sicakligi: %.1f C\n", motor_sicakligi);
    printf("Batarya Sicakligi: %.1f C\n", batarya_sicakligi);
    printf("-----------------------\n");

    float toplam = 0.0f;
    float ort = 0.0f;

    printf("\n--- SURUS ISTATISTIKLERI ---\n");

    toplam = 0.0f;
    for (int i = 0; i < hizlanma_sayac; i++) toplam += hizlanma_kayitlari[i];
    ort = hizlanma_sayac > 0 ? toplam / hizlanma_sayac : 0.0f;
    printf("Gaza Basma Sayisi: %d (Ortalama Artis: %.2f km/s)\n", hizlanma_sayac, ort);

    toplam = 0.0f;
    for (int i = 0; i < yavaslama_sayac; i++) toplam += yavaslama_kayitlari[i];
    ort = yavaslama_sayac > 0 ? toplam / yavaslama_sayac : 0.0f;
    printf("Frene Basma Sayisi: %d (Ortalama Dusus: %.2f km/s)\n", yavaslama_sayac, ort);

    toplam = 0.0f;
    for (int i = 0; i < rejen_sayac; i++) toplam += rejen_kayitlari[i];
    ort = rejen_sayac > 0 ? toplam / rejen_sayac : 0.0f;
    printf("Rejeneratif Frenleme Sayisi: %d (Ortalama Dusus: %.2f km/s)\n", rejen_sayac, ort);

    printf("-----------------------\n");
}

void sistemi_kapat() {
    printf("\n[BILGI] Sistem kapatiliyor... Son Surus Istatistikleri:\n");
    telemetri_ve_istatistik_yazdir();
    printf("[BILGI] Motor guvenli bir sekilde kapatildi. Iyi gunler!\n");
}

void gaza_bas() {
    if (batarya <= 0) {
        printf("UYARI: Batarya Tukendi! Arac hareket ettirilemiyor.\n");
        return;
    }

    float artis = (rand() % 81 + 20) / 10.0f;
    hiz += artis;
    if (hiz > MAX_HIZ) hiz = MAX_HIZ;

    batarya -= 2;
    if (batarya < 0) batarya = 0;

    motor_sicakligi += 5.0f;
    batarya_sicakligi += 2.0f;

    if (hizlanma_sayac < MAX_KAYIT) {
        hizlanma_kayitlari[hizlanma_sayac] = artis;
        hizlanma_sayac++;
    }

    printf("[BILGI] Gaza basildi. Arac %.1f km/s hizlandi. Motor: %.1fC, Batarya: %.1fC\n",
           artis, motor_sicakligi, batarya_sicakligi);

    if (motor_sicakligi >= KRITIK_MOTOR_SICAKLIGI || batarya_sicakligi >= KRITIK_BATARYA_SICAKLIGI) {
        printf("KRITIK HATA: Asiri Isinma! Sistem acil kapatiliyor.\n");
        sistemi_kapat();
        exit(1);
    }
}

void frene_bas() {
    if (hiz == 0.0f) {
        printf("UYARI: Arac zaten duruyor!\n");
        return;
    }

    float dusus = (rand() % 101 + 50) / 10.0f;
    hiz -= dusus;
    if (hiz < 0.0f) hiz = 0.0f;

    motor_sicakligi -= 3.0f;
    batarya_sicakligi -= 1.0f;
    if (motor_sicakligi < ORTAM_SICAKLIGI) motor_sicakligi = ORTAM_SICAKLIGI;
    if (batarya_sicakligi < ORTAM_SICAKLIGI) batarya_sicakligi = ORTAM_SICAKLIGI;

    if (yavaslama_sayac < MAX_KAYIT) {
        yavaslama_kayitlari[yavaslama_sayac] = dusus;
        yavaslama_sayac++;
    }

    printf("[BILGI] Frene basildi. Arac %.1f km/s yavasladi. Motor: %.1fC, Batarya: %.1fC\n",
           dusus, motor_sicakligi, batarya_sicakligi);
}

void rejen_fren() {
    if (hiz == 0.0f) {
        printf("UYARI: Arac zaten duruyor!\n");
        return;
    }

    float dusus = (rand() % 81 + 20) / 10.0f;
    hiz -= dusus;
    if (hiz < 0.0f) hiz = 0.0f;

    batarya += 1;
    if (batarya > 100) batarya = 100;

    motor_sicakligi -= 2.0f;
    batarya_sicakligi += 1.0f;
    if (motor_sicakligi < ORTAM_SICAKLIGI) motor_sicakligi = ORTAM_SICAKLIGI;

    if (rejen_sayac < MAX_KAYIT) {
        rejen_kayitlari[rejen_sayac] = dusus;
        rejen_sayac++;
    }

    printf("[BILGI] Rejeneratif fren devrede. Arac %.1f km/s yavasladi. Batarya sarj oluyor.\n", dusus);

    if (batarya_sicakligi >= KRITIK_BATARYA_SICAKLIGI) {
        printf("KRITIK HATA: Asiri Isinma! Sistem acil kapatiliyor.\n");
        sistemi_kapat();
        exit(1);
    }
}