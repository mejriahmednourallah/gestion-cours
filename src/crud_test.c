#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "membres.h"
#include "centres.h"

static int copy_file(const char* src, const char* dst) {
    FILE* in = fopen(src, "r");
    if (!in) return 0;
    FILE* out = fopen(dst, "w");
    if (!out) { fclose(in); return 0; }
    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
        if (fwrite(buf, 1, n, out) != n) { fclose(in); fclose(out); return 0; }
    }
    fclose(in);
    fclose(out);
    return 1;
}

static void print_header(const char* title) {
    printf("\n==========================================\n");
    printf("%s\n", title);
    printf("==========================================\n");
}

static int test_membres_delete_restore(void) {
    const char* data = "data/membres.txt";
    const char* backup = "data/membres.txt.bak";
    print_header("TEST MEMBRES - Suppression puis restauration");

    if (!copy_file(data, backup)) {
        printf("❌ Backup membres impossible\n");
        return 0;
    }

    Membre* arr = NULL; int count_before = charger_membres(&arr);
    printf("Avant suppression: %d membres\n", count_before);
    if (count_before <= 0 || arr == NULL) {
        remove(backup);
        printf("⚠️  Rien à tester (aucun membre)\n");
        return 1;
    }

    int id_to_delete = arr[0].id;
    free(arr);

    supprimer_membre(id_to_delete);

    Membre* arr2 = NULL; int count_after = charger_membres(&arr2);
    if (arr2) free(arr2);
    printf("Après suppression: %d membres\n", count_after);

    int ok = (count_after == count_before - 1);
    printf(ok ? "✅ Suppression membre OK (id %d)\n" : "❌ Suppression membre KO (id %d)\n", id_to_delete);

    // Restore
    if (!copy_file(backup, data)) {
        printf("❌ Restauration membres échouée\n");
        remove(backup);
        return 0;
    }
    remove(backup);
    return ok;
}

static int test_centres_delete_restore(void) {
    const char* data = "data/centres.txt";
    const char* backup = "data/centres.txt.bak";
    print_header("TEST CENTRES - Suppression puis restauration");

    if (!copy_file(data, backup)) {
        printf("❌ Backup centres impossible\n");
        return 0;
    }

    Centre* arr = NULL; int count_before = charger_centres(&arr);
    printf("Avant suppression: %d centres\n", count_before);
    if (count_before <= 0 || arr == NULL) {
        remove(backup);
        printf("⚠️  Rien à tester (aucun centre)\n");
        return 1;
    }

    int id_to_delete = arr[0].id;
    free(arr);

    supprimer_centre(id_to_delete);

    Centre* arr2 = NULL; int count_after = charger_centres(&arr2);
    if (arr2) free(arr2);
    printf("Après suppression: %d centres\n", count_after);

    int ok = (count_after == count_before - 1);
    printf(ok ? "✅ Suppression centre OK (id %d)\n" : "❌ Suppression centre KO (id %d)\n", id_to_delete);

    // Restore
    if (!copy_file(backup, data)) {
        printf("❌ Restauration centres échouée\n");
        remove(backup);
        return 0;
    }
    remove(backup);
    return ok;
}

int main(void) {
    int ok1 = test_membres_delete_restore();
    int ok2 = test_centres_delete_restore();
    printf("\nRésultat global: %s\n", (ok1 && ok2) ? "✅ SUCCÈS" : "❌ ECHEC");
    return (ok1 && ok2) ? 0 : 1;
}
