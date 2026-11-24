#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Yardımcı: Metin içindeki id("ad") ifadelerini geçerli C değişken adına (id_ad) çevirir
void id_degiskene_cevir(const char *girdi, char *cikti) {
    cikti[0] = '\0';
    const char *pos = girdi;
    const char *bul;
    while ((bul = strstr(pos, "id(\"")) != NULL) {
        strncat(cikti, pos, bul - pos);
        char isim[64];
        if (sscanf(bul + 4, "%63[^\"]", isim) == 1) {
            strcat(cikti, "id_");
            strcat(cikti, isim);
            pos = strstr(bul + 4, "\")");
            pos = pos ? pos + 2 : bul + 4 + strlen(isim);
        } else {
            break;
        }
    }
    strcat(cikti, pos);
}

// 1. Genel Zincirleme Eşitsizlik Çözücü (Herhangi bir 'A op1 B op2 C' aralığı için)
void genel_esitsizlik_coz(const char *girdi, char *cikti) {
    char temiz[256];
    id_degiskene_cevir(girdi, temiz);

    char sol[64], op1[8], orta[64], op2[8], sag[64];
    if (sscanf(temiz, "%63s %7[<>=!] %63s %7[<>=!] %63s", sol, op1, orta, op2, sag) == 5) {
        sprintf(cikti, "(%s %s %s && %s %s %s)", sol, op1, orta, orta, op2, sag);
    } else {
        strcpy(cikti, temiz);
    }
}

// 2. Genel Koşul Ayrıştırıcı
void kosul_ayristir(char *satir, char *kosul_cikti, char *hedef_blok) {
    hedef_blok[0] = '\0';

    char *ret_blok = strstr(satir, "-> return block(");
    if (ret_blok) {
        *ret_blok = '\0';
        char ham[64];
        sscanf(ret_blok + 16, "%63[^)]", ham);
        if (ham[0] == '"') sscanf(ham + 1, "%63[^\"]", hedef_blok);
        else strcpy(hedef_blok, ham);
    } else {
        char *son_ok = NULL, *ara = satir;
        while ((ara = strstr(ara, "->")) != NULL) {
            son_ok = ara;
            ara += 2;
        }
        if (son_ok) *son_ok = '\0';
    }

    char ara_tampon[256] = "";
    char *pos = satir, *bulunan;
    while ((bulunan = strstr(pos, "-> and")) != NULL) {
        strncat(ara_tampon, pos, bulunan - pos);
        strcat(ara_tampon, "&&");
        pos = bulunan + 6;
    }
    strcat(ara_tampon, pos);

    genel_esitsizlik_coz(ara_tampon, kosul_cikti);
}

// 3. Genel Frange Ayrıştırıcı: frange.(baslangic-bitis) -> Herhangi iki değişken/kimlik/sayı
void genel_frange_yaz(const char *satir, FILE *cikis) {
    const char *f_bas = strstr(satir, "frange.(");
    if (!f_bas) return;

    char icerik[128];
    sscanf(f_bas + 8, "%127[^)]", icerik); // Örn: id("m")-id("a") veya x-y veya 1-50

    // Parantez içindeki '-' işaretini bul (tırnak dışındaki)
    char *tire = NULL;
    int tirnak_icinde = 0;
    for (char *c = icerik; *c; c++) {
        if (*c == '"') tirnak_icinde = !tirnak_icinde;
        if (*c == '-' && !tirnak_icinde) {
            tire = c;
            break;
        }
    }

    if (tire) {
        *tire = '\0';
        char bas_c[64], bit_c[64];
        id_degiskene_cevir(icerik, bas_c);
        id_degiskene_cevir(tire + 1, bit_c);

        // Hem artan hem azalan aralıkları otomatik destekleyen genel döngü
        fprintf(cikis, "        double _cm_step = (%s <= %s) ? 1.0 : -1.0;\n", bas_c, bit_c);
        fprintf(cikis, "        for (double i = %s; (_cm_step > 0) ? (i <= %s) : (i >= %s); i += _cm_step) {\n",
                bas_c, bit_c, bit_c);
        fprintf(cikis, "            printf(\"[CM Frange]: %%.2f\\n\", i);\n");
        fprintf(cikis, "        }\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Kullanim: ./cm <dosya.cm>\n");
        return 1;
    }

    srand((unsigned int)time(NULL));
    FILE *giris = fopen(argv[1], "r");
    FILE *cikis = fopen("/tmp/_cm_gecici.c", "w");
    if (!giris || !cikis) {
        printf("Dosya acilamadi!\n");
        return 1;
    }

    char satir[256], kosul[256], hedef_blok[64], donusmus[256];

    fprintf(cikis, "#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n#include <math.h>\n#include <sys/stat.h>\n\n");
    fprintf(cikis, "void cm_save_target(char *target_id) {\n");
    fprintf(cikis, "    if (strlen(target_id) == 0) {\n");
    fprintf(cikis, "        mkdir(\"cm_hedef_klasoru\", 0777);\n");
    fprintf(cikis, "        strcpy(target_id, \"cm_hedef_klasoru/varsayilan.dat\");\n");
    fprintf(cikis, "    }\n");
    fprintf(cikis, "    FILE *f = fopen(target_id, \"wb\");\n");
    fprintf(cikis, "    if (f) { fwrite(\"CM_OK\", 1, 5, f); fclose(f); }\n");
    fprintf(cikis, "    printf(\"[CM Depo]: '%%s' kaydedildi.\\n\", target_id);\n");
    fprintf(cikis, "}\n\n");

    fprintf(cikis, "int main(void) {\n");
    fprintf(cikis, "    char target[128] = \"\";\n\nmain:\n");

    while (fgets(satir, sizeof(satir), giris)) {
        char *yorum = strstr(satir, "//");
        if (yorum) *yorum = '\0';

        char *p = satir;
        while (isspace((unsigned char)*p)) p++;
        if (*p == '\0' || strncmp(p, "call ", 5) == 0) continue;
        if (strstr(p, "...") != NULL) continue;

        // 1. 'end.' -> Program sonu
        if (strncmp(p, "end.", 4) == 0) {
            fprintf(cikis, "    }\n    return 0;\n}\n");
            break;
        }
        // 2. 'end' -> Blok sonu
        else if (strncmp(p, "end", 3) == 0) {
            fprintf(cikis, "    }\n\n");
        }
        // 3. 'else'
        else if (strncmp(p, "else", 4) == 0) {
            fprintf(cikis, "    } else {\n");
        }
        // 4. 'if ... ->' (Genel aralık ve mantık koşulu)
        else if (strncmp(p, "if ", 3) == 0) {
            kosul_ayristir(p + 3, kosul, hedef_blok);
            fprintf(cikis, "    if (%s) {\n", kosul);
            if (hedef_blok[0] != '\0') {
                fprintf(cikis, "        goto %s;\n", hedef_blok);
            }
        }
        // 5. 'loop(frange.(...))' (Tam genel aralık döngüsü)
        else if (strncmp(p, "loop(", 5) == 0 && strstr(p, "frange.(")) {
            genel_frange_yaz(p, cikis);
        }
        // 6. 'make.(a == b)' (Genel zorlama/eşitleme)
        else if (strncmp(p, "make.(", 6) == 0) {
            char sol[64], sag[64], sol_c[64], sag_c[64];
            if (sscanf(p + 6, "%63[^=]==%63[^)]", sol, sag) == 2) {
                id_degiskene_cevir(sol, sol_c);
                id_degiskene_cevir(sag, sag_c);
                fprintf(cikis, "        %s = %s;\n", sol_c, sag_c);
            }
        }
        // 7. 'href "link" show("...") id("...") /href'
        else if (strncmp(p, "href ", 5) == 0 && strstr(p, "/href")) {
            char link[128] = "", gorunen[128] = "";
            char *t1 = strchr(p + 5, '"');
            if (t1) sscanf(t1 + 1, "%127[^\"]", link);
            char *s_pos = strstr(p, "show(\"");
            if (s_pos) sscanf(s_pos + 6, "%127[^\"]", gorunen);
            else strcpy(gorunen, link);

            fprintf(cikis, "    strcpy(target, \"%s\");\n", link);
            fprintf(cikis, "    printf(\"[CM Href]: %s -> (%s)\\n\");\n", gorunen, link);
        }
        // 8. 'save(target)'
        else if (strncmp(p, "save(", 5) == 0) {
            fprintf(cikis, "        cm_save_target(target);\n");
        }
        // 9. 'print ...' (Her türlü matematiksel ifadeyi hesaplar)
        else if (strncmp(p, "print ", 6) == 0) {
            p[strcspn(p, "\r\n")] = 0;
            id_degiskene_cevir(p + 6, donusmus);
            fprintf(cikis, "        printf(\"[CM Print]: %%.2f\\n\", (double)(%s));\n", donusmus);
        }
        // 10. Genel Değişken ve Kimlik Atamaları (Örn: x = 10, y = 20, id("m") = 3, id("a") = 8)
        else if (strchr(p, '=')) {
            p[strcspn(p, "\r\n")] = 0;
            // target = id("") özel durumunu ayıkla
            char *t_pos = strstr(p, ", target");
            if (t_pos) *t_pos = '\0';

            id_degiskene_cevir(p, donusmus);
            fprintf(cikis, "    double %s;\n", donusmus);
        }
    }

    fclose(giris);
    fclose(cikis);

    int durum = system("gcc /tmp/_cm_gecici.c -lm -o /tmp/cm_program");
//    remove("/tmp/_cm_gecici.c");
    if (durum == 0) {
        system("/tmp/cm_program");
    }
    return 0;
}
