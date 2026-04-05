#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "telemetry.h"

int main() {
    float batarya_sicakligi;
    int sarj_yuzdesi;
    char kapi_durumu;
    int fren_pedali;

    srand(time(NULL));

    printf("--- ELEKTROMOBIL GUVENLIK KONTROLU ---\n");

    printf("Batarya Sicakligi (C): ");
    if (scanf("%f", &batarya_sicakligi) != 1) {
        printf("SONUC: HATA: Gecersiz giris! Sayi bekleniyor.\n");
        while (getchar() != '\n');
        return 1;
    }
    while (getchar() != '\n');

    if (batarya_sicakligi < -100 || batarya_sicakligi > 200) {
        printf("SONUC: Mantiksiz sicaklik (%.1f). Lutfen sicaklik verilerini kontrol edin.\n", batarya_sicakligi);
        return 1;
    }

    printf("Sarj Yuzdesi (%%): ");
    if (scanf("%d", &sarj_yuzdesi) != 1) {
        printf("SONUC: HATA: Gecersiz giris! Tam sayi bekleniyor.\n");
        while (getchar() != '\n');
        return 1;
    }
    while (getchar() != '\n');

    if (sarj_yuzdesi < 0 || sarj_yuzdesi > 100) {
        printf("SONUC: HATA: Imkansiz sarj seviyesi (%%%d). Lutfen sensor verilerini kontrol edin.\n", sarj_yuzdesi);
        return 1;
    }

    printf("Kapi Durumu (A/K): ");
    if (scanf(" %c", &kapi_durumu) != 1) {
        printf("SONUC: HATA: Gecersiz giris!\n");
        while (getchar() != '\n');
        return 1;
    }
    while (getchar() != '\n');

    if (kapi_durumu != 'A' && kapi_durumu != 'K') {
        printf("SONUC: HATA: Gecersiz karakter (%c). Yalnizca 'A' veya 'K' girin.\n", kapi_durumu);
        return 1;
    }

    printf("Fren Pedali (1:Basili, 0:Degil): ");
    if (scanf("%d", &fren_pedali) != 1) {
        printf("SONUC: HATA: Gecersiz giris! 0 veya 1 bekleniyor.\n");
        while (getchar() != '\n');
        return 1;
    }
    while (getchar() != '\n');

    if (fren_pedali != 0 && fren_pedali != 1) {
        printf("SONUC: UYARI: Gecersiz fren pedali girisi (%d). Yalnizca 0 veya 1 girin.\n", fren_pedali);
        return 1;
    }

    if (batarya_sicakligi > 60) {
        printf("SONUC: KRITIK HATA: Motor Asiri Isindi! Surus Engellendi.\n");
        return 1;
    }

    if (sarj_yuzdesi < 10) {
        printf("SONUC: UYARI: Batarya Kritik Seviyede! Surus Baslatilamaz.\n");
        return 1;
    }

    if (kapi_durumu == 'A') {
        printf("SONUC: HATA: Kapilar Acik! Lutfen Kapatiniz.\n");
        return 1 ;
    }

    if (fren_pedali == 0) {
        printf("SONUC: BILGI: Guvenlik icin frene basarak tekrar deneyin.\n");
        return 1;
    }

    printf("SONUC: BASARILI: SISTEM HAZIR. MOTOR BASLATILIYOR...\n");

    sistemi_kur(batarya_sicakligi, sarj_yuzdesi);

    int secim;
    while (1) {
        printf("\n--- TUFAN ELEKTROMOBIL SURUS SIMULASYONU ---\n");
        printf("1. Gaza Bas\n");
        printf("2. Frene Bas\n");
        printf("3. Rejeneratif Frenleme Yap\n");
        printf("4. Anlik Telemetri ve Istatistikleri Oku\n");
        printf("5. Sistemi Kapat\n");
        printf("Seciminiz: ");

        if (scanf("%d", &secim) != 1) {
            printf("HATA: Gecersiz giris! Lutfen 1-5 arasi bir sayi girin.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (secim) {
            case 1: gaza_bas();                       break;
            case 2: frene_bas();                      break;
            case 3: rejen_fren();                     break;
            case 4: telemetri_ve_istatistik_yazdir(); break;
            case 5: sistemi_kapat(); return 0;
            default:
                printf("HATA: Gecersiz secim (%d)! Lutfen 1-5 arasi girin.\n", secim);
                break;
        }
    }

    return 0;
}