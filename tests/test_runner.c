// tests/test_runner.c - 最終版
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#define XPT_ON
#include "xpt.h"

static volatile int interrupted = 0;

void signal_handler(int signum) {
    interrupted = 1;
    fprintf(stderr, "\n\nTest interrupted by user (Ctrl+C)\n");
}

// ===== バージョンテスト関数 =====

int test_version(const char *command, const char *expected_version) {
    FILE *fp;
    char output[1024];
    char cmd[512];

    if (interrupted) {
        return -1;
    }

    // XPT_ERR | XPT_WRN を有効化
    snprintf(cmd, sizeof(cmd), "XPT_FLG=0x0003 %s 2>&1", command);

    fp = popen(cmd, "r");
    if (!fp) {
        fprintf(stderr, "  FAILED: Command execution error\n");
        return 1;
    }

    // 最初の1行を読む
    if (!fgets(output, sizeof(output), fp)) {
        pclose(fp);
        fprintf(stderr, "  FAILED: No output from command\n");
        return 1;
    }

    output[strcspn(output, "\r\n")] = '\0';

    // バージョンチェック
    char expected[256];
    snprintf(expected, sizeof(expected), "version: %s", expected_version);

    if (strstr(output, expected)) {
        // 成功: すぐに終了（子プロセスも強制終了）
        pclose(fp);
        return 0;
    } else {
        // 失敗: エラー詳細を表示
        XPT(XPT_ERR, "ERR: mismatch %s\n", output);
        pclose(fp);
        return 1;
    }
}

int test_0001_version_oasis_layer1(void) {
    return test_version("oasis_layer1", "v1.6.1");
}

int test_0002_version_oasis_layer2(void) {
    return test_version("oasis_layer2", "v1.6.1");
}

int test_0003_version_oasis_layer3(void) {
    return test_version("oasis_layer3", "v1.6.1");
}

int test_0004_version_oasis_divs(void) {
    return test_version("oasis_divs", "v1.6.1");
}

int test_0005_version_prime_oasis(void) {
    return test_version("prime_oasis", "v1.6.1");
}

#if 0	// for test.
int test_0006_version_prime_oases(void) {
    FILE *fp;
    char output[1024];
    char cmd[512];

    if (interrupted) {
        return -1;
    }

    snprintf(cmd, sizeof(cmd), "XPT_FLG=0x0003 prime_oases 1 2>&1");

    fp = popen(cmd, "r");
    if (!fp) {
        fprintf(stderr, "  FAILED: Command execution error\n");
        return 1;
    }

    if (!fgets(output, sizeof(output), fp)) {
        pclose(fp);
        fprintf(stderr, "  FAILED: No output from command\n");
        return 1;
    }

    if (strstr(output, "version: v1.6.1")) {
        pclose(fp);
        return 0;
    } else {
        fprintf(stderr, "  FAILED: Version mismatch\n");
        fprintf(stderr, "  Expected: version: v1.6.1\n");
        fprintf(stderr, "  Actual:   %s", output);

        fprintf(stderr, "\n--- Error/Warning Trace ---\n");
        int line_count = 0;
        while (fgets(output, sizeof(output), fp) && line_count < 100) {
            if (strstr(output, "[ERROR]") || strstr(output, "[WARN]")) {
                fprintf(stderr, "%s", output);
                line_count++;
            }
        }
        fprintf(stderr, "--- End Trace ---\n");

        pclose(fp);
        return 1;
    }
}
#else
int test_0006_version_prime_oases(void) {
    return test_version("prime_oases", "v1.6.1");
}
#endif

typedef struct {
    int number;
    const char *description;
    int (*func)(void);
} Test;

Test tests[] = {
    {1, "oasis_layer1:ver", test_0001_version_oasis_layer1},
    {2, "oasis_layer2:ver", test_0002_version_oasis_layer2},
    {3, "oasis_layer3:ver", test_0003_version_oasis_layer3},
    {4, "oasis_divs:ver",   test_0004_version_oasis_divs},
    {5, "prime_oasis:ver",  test_0005_version_prime_oasis},
    {6, "prime_oases:ver",  test_0006_version_prime_oases},
    {0, NULL, NULL}  // 終端マーカー
};

int xpt_flg = XPT_ERR | XPT_WRN;
//int xpt_flg = 0;

int main(int argc, char *argv[]) {
    int passed = 0;
    int failed = 0;
    int total_tests = 0;

    XPT_INIT();

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    for (int i = 0; tests[i].func != NULL; i++) {
        total_tests++;
    }

    for (int i = 0; tests[i].func != NULL; i++) {
        if (interrupted) {
            return 130;  // 128 + SIGINT(2)
        }

        printf("====< %04d %s\n", tests[i].number, tests[i].description);
        fflush(stdout);

        int result = tests[i].func();

        if (result == -1) {
            return 130;
        } else if (result == 0) {
            passed++;
        } else {
            failed++;
            return 1;
        }
    }

    return 0;
}
