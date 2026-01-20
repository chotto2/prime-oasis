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

// ===== テスト関数 =====
int test_0001(void) {
    int ret = 0;
    const char *command="oasis_layer1";
    FILE *fp;
    char output[1024];
    char cmd[512];
static const char *expected_output[] = { "oasis prime  = 2825316306925682433915768672179340796128917213519487069241249529171862110997815815759607546544691213214701362889014226079377908856929026235806002962070710365662543644390862362492580972116364262478035480289616321859707554022310668332270492441122793125806189337411281492496035721393388592871507609346222719999", "oasis prime  = 3585978389559520012277706391612240241240548771005502818652355171641209602420304689233348039845185001387890191359133440793056576626102225606984542221089747771802459240957632998548275849224616179299045032675282254668090357028317386729420240406040468198138624928252780355860353030999300906336913504170205759999", "oasis prime  = 5215968566632029108767572933254167623622616394189822281676152976932668512611352275248506239774814547473294823795103186608082293274330509973795697776130542213530849805029284361524764871599441715344065502073137824971767792041188926151883986045149771924565272622913135063069604408726255863762783278793026559999", "(try=1402, hit=20, twin=0)" };



    XPT(XPT_SNP, "SNP:test_0001: Start.\n");
    if (interrupted) {
        XPT(XPT_WRN, "WRN:test_0001: interrupted.\n");
        ret = -1;
    }
    else {

        snprintf(cmd, sizeof(cmd), "XPT_FLG=0x0003 %s 2>&1", command);

        fp = popen(cmd, "r");
        if (fp == NULL) {
            XPT(XPT_ERR, "ERR:test_0001: %s\n", command);
            ret = 1;
        }
        else {
	    int exp_idx = 0;
	    for (int i = 0; i < 21; i++) {
                if (!fgets(output, sizeof(output), fp)) {
                    XPT(XPT_ERR, "ERR:test_0001: 0 = fgets(fp)\n");
                    ret = 2;
                }
	        else {

		    switch (i) {
		    case 0:	// oasis_layer1 has no key interrupt.
		    case 9:
	            case 19:
                    case 20:
                       int len = strlen(expected_output[exp_idx]);
                       XPT(XPT_SNP, "SNP: output = '%s'", output);
                       XPT(XPT_SNP, "SNP: expected_output[exp_idx=%d] = '%s'\n", exp_idx, expected_output[exp_idx]);

		       if (strncmp(output, expected_output[exp_idx], len) != 0) {
			       XPT(XPT_ERR, "ERR:line=%d: %s", i+1, output);
                               ret = 3;
		       }
		       else {
			       XPT(XPT_SNP, "SNP:test_0001:OK: %s\n", output);
			       exp_idx++;
		       }
		       XPT(XPT_SNP, "(4)\n");
		       break;

		    default:
			       XPT(XPT_SNP, "SNP:test_0001: %s\n", output);
		       break;
		    }
	        }
		if (ret) break;
	    }
            pclose(fp);
        }
    }

    XPT(XPT_SNP, "SNP: ret = %d\n", ret);
    return ret;
}

int test_0002(void) {
    int ret = 0;
    const char *command="oasis_layer2";
    FILE *fp;
    char output[1024];
    char cmd[512];
static const char *expected_output[] = { 
	"oasis prime  = 2627072279800520541108487733245775976709705914990377662819691566645970417005220318148142724703856185033686932519587335291395440838811257152578688808732448765114590478925747900600954691097754312058906587230259388083365289208029989861267516201208765727131254172910746710700018581925544158406797303706682880001",
        "oasis prime  = 3944133191778885610347690640579502006878183451074217982663642407019172530182462215103045974308821718557085992076063739383621653085225380446962506493903691749355680498123627168485259299698825145745026924460701243812082850167679386044873316934231336615455858989464192925839059306245012415803807796980936320001",
        "oasis prime  = 5253336573386213982210731009467067540757519426581202907521411358675814189708101976382063986022016095128948044421935089458894350474200567054394182973095747112304741846804548428401924363434313458008823344462547219103087189897614555435315287245140131430125450373845165950463782266050861039043058969918418239999",
        "(try=968782, hit=16093, twin=143)" };



    XPT(XPT_SNP, "SNP:test_0002: Start.\n");
    if (interrupted) {
        XPT(XPT_WRN, "WRN:test_0002: interrupted.\n");
        ret = -1;
    }
    else {

        snprintf(cmd, sizeof(cmd), "XPT_FLG=0x0003 %s 2>&1", command);

        fp = popen(cmd, "r");
        if (fp == NULL) {
            XPT(XPT_ERR, "ERR:test_0002: %s\n", command);
            ret = 1;
        }
        else {
	    int exp_idx = 0;
	    for (int i = 0; i < 16097; i++) {
                if (!fgets(output, sizeof(output), fp)) {
                    XPT(XPT_ERR, "ERR:test_0002: 0 = fgets(fp)\n");
                    ret = 2;
                }
	        else {

		    switch (i) {
		    case 3:	// oasis_layer2 has key interrupt.
	            case 8099:
                    case 16095:
                    case 16096:
                       int len = strlen(expected_output[exp_idx]);
                       XPT(XPT_SNP, "SNP:test_0002: output = '%s'", output);
                       XPT(XPT_SNP, "SNP:test_0002: expected_output[exp_idx=%d] = '%s'\n", exp_idx, expected_output[exp_idx]);

		       if (strncmp(output, expected_output[exp_idx], len) != 0) {
			       XPT(XPT_ERR, "ERR:test_0002:line=%d: %s", i+1, output);
                               ret = 3;
		       }
		       else {
			       XPT(XPT_SNP, "SNP:test_0002:OK: %s\n", output);
			       exp_idx++;
		       }
		       XPT(XPT_SNP, "(4)\n");
		       break;

		    default:
			       XPT(XPT_SNP, "SNP:test_0002: %s\n", output);
		       break;
		    }
	        }
		if (ret) break;
	    }
            pclose(fp);
        }
    }

    XPT(XPT_SNP, "SNP:test_0002: ret = %d\n", ret);
    return ret;
}

int test_0005(void) {
    int ret = 0;
    const char *command="prime_oasis 701 683";
    FILE *fp;
    char output[1024];
    char cmd[512];
static const char *expected_output[] = { 
        "oasis prime  = 2627072279800520541108487733245775976709705914990377662819691566645970417005220318148142724703856185033686932519587335291395440838811257152578688808732448765114590478925747900600954691097754312058906587230259388083365289208029989861267516201208765727131254172910746710700018581925544158406797303706682880001",
        "oasis prime  = 3944133191778885610347690640579502006878183451074217982663642407019172530182462215103045974308821718557085992076063739383621653085225380446962506493903691749355680498123627168485259299698825145745026924460701243812082850167679386044873316934231336615455858989464192925839059306245012415803807796980936320001",
        "oasis prime  = 5253336573386213982210731009467067540757519426581202907521411358675814189708101976382063986022016095128948044421935089458894350474200567054394182973095747112304741846804548428401924363434313458008823344462547219103087189897614555435315287245140131430125450373845165950463782266050861039043058969918418239999",
        "(try=968782, hit=16093, twin=143)" };



    XPT(XPT_SNP, "SNP:test_0005: Start.\n");
    if (interrupted) {
        XPT(XPT_WRN, "WRN:test_0005: interrupted.\n");
        ret = -1;
    }
    else {

        snprintf(cmd, sizeof(cmd), "XPT_FLG=0x0003 %s 2>&1", command);

        fp = popen(cmd, "r");
        if (fp == NULL) {
            XPT(XPT_ERR, "ERR:test_0005: %s\n", command);
            ret = 1;
        }
        else {
	    int exp_idx = 0;
	    for (int i = 0; i < 16097; i++) {
                if (!fgets(output, sizeof(output), fp)) {
                    XPT(XPT_ERR, "ERR:test_0005: 0 = fgets(fp)\n");
                    ret = 2;
                }
	        else {

		    switch (i) {
		    case 3:	// prime_oasis has key interrupt.
	            case 8099:
                    case 16095:
                    case 16096:
                       int len = strlen(expected_output[exp_idx]);
                       XPT(XPT_SNP, "SNP:test_0005: output = '%s'", output);
                       XPT(XPT_SNP, "SNP:test_0005: expected_output[exp_idx=%d] = '%s'\n", exp_idx, expected_output[exp_idx]);

		       if (strncmp(output, expected_output[exp_idx], len) != 0) {
			       XPT(XPT_ERR, "ERR:test_0005:line=%d: %s", i+1, output);
                               ret = 3;
		       }
		       else {
			       XPT(XPT_SNP, "SNP:test_0005:OK: %s\n", output);
			       exp_idx++;
		       }
		       XPT(XPT_SNP, "(4)\n");
		       break;

		    default:
			       XPT(XPT_SNP, "SNP:test_0005: %s\n", output);
		       break;
		    }
	        }
		if (ret) break;
	    }
            pclose(fp);
        }
    }

    XPT(XPT_SNP, "SNP:test_0005: ret = %d\n", ret);
    return ret;
}

int test_0006(void) {
    int ret = 0;
    const char *command="prime_oases d683 x484391 484391";
    FILE *fp;
    char output[1024];
    char cmd[512];
static const char *expected_output[] = { 
     "d683*484456+1 = 2627072279800520541108487733245775976709705914990377662819691566645970417005220318148142724703856185033686932519587335291395440838811257152578688808732448765114590478925747900600954691097754312058906587230259388083365289208029989861267516201208765727131254172910746710700018581925544158406797303706682880001",
     "d683*727334+1 = 3944133191778885610347690640579502006878183451074217982663642407019172530182462215103045974308821718557085992076063739383621653085225380446962506493903691749355680498123627168485259299698825145745026924460701243812082850167679386044873316934231336615455858989464192925839059306245012415803807796980936320001",
     "d683*968763-1 = 5253336573386213982210731009467067540757519426581202907521411358675814189708101976382063986022016095128948044421935089458894350474200567054394182973095747112304741846804548428401924363434313458008823344462547219103087189897614555435315287245140131430125450373845165950463782266050861039043058969918418239999",
     "{ prime_oases d683 x484391 484391: try=968782, hit=16093(1.7%) }" };

    XPT(XPT_SNP, "SNP:test_0006: Start.\n");
    if (interrupted) {
        XPT(XPT_WRN, "WRN:test_0006: interrupted.\n");
        ret = -1;
    }
    else {

        snprintf(cmd, sizeof(cmd), "XPT_FLG=0x0003 %s 2>&1", command);

        fp = popen(cmd, "r");
        if (fp == NULL) {
            XPT(XPT_ERR, "ERR:test_0006: %s\n", command);
            ret = 1;
        }
        else {
	    int exp_idx = 0;
	    for (int i = 0; i < 16097; i++) {
                if (!fgets(output, sizeof(output), fp)) {
                    XPT(XPT_ERR, "ERR:test_0006: 0 = fgets(fp)\n");
                    ret = 2;
                }
	        else {

		    switch (i) {
		    case 3:	// prime_oases has key interrupt.
	            case 8099:
                    case 16095:
                    case 16096:
                       int len = strlen(expected_output[exp_idx]);
                       XPT(XPT_SNP, "SNP:test_0006: output = '%s'", output);
                       XPT(XPT_SNP, "SNP:test_0006: expected_output[exp_idx=%d] = '%s'\n", exp_idx, expected_output[exp_idx]);

		       if (strncmp(output, expected_output[exp_idx], len) != 0) {
			       XPT(XPT_ERR, "ERR:test_0006:line=%d: %s", i+1, output);
                               ret = 3;
		       }
		       else {
			       XPT(XPT_SNP, "SNP:test_0006:OK: %s\n", output);
			       exp_idx++;
		       }
		       XPT(XPT_SNP, "(4)\n");
		       break;

		    default:
			       XPT(XPT_SNP, "SNP:test_0006: %s\n", output);
		       break;
		    }
	        }
		if (ret) break;
	    }
            pclose(fp);
        }
    }

    XPT(XPT_SNP, "SNP:test_0006: ret = %d\n", ret);
    return ret;
}

int test_0003(void) {
    int ret = 0;
    const char *command="oasis_layer3";
    FILE *fp;
    char output[1024];
    char cmd[512];
static const char *expected_output[] = { 
        "oasis prime  = 2626719993142388796492088774814915306018609313624104309529209135096527519657955795473268545924028447011608143849315661231548801102678373000902667018119543076691768858987417185550809405617657711830718788171232434763812577700842572318761327433007329932905371526526735919255713786949826248456062421364781119999",
        "oasis prime  = 2630568842586602575180928113135247631401889321988252084605736312247376823510443414144324097945383226157606436114765141537238009180153885935920623146938750774633090692037488269669464151997398278854634025020669637144036797391997381253552973255356919238877575865448196858762886193494389584215857049457883199999",
        "oasis prime  = 2634376692091608485460241314534235734129632541696398516543906200256189141000187934956574343365150697396502908166712944512124484671088860822229851130646848851988546737853520458732362880838223364064338137614606283148651492794244664457811118394850764603054858792052445390908980644589343776909559073543557439999",
        "(try=1928792, hit=32000, twin=282)" };



    XPT(XPT_SNP, "SNP:test_0003: Start.\n");
    if (interrupted) {
        XPT(XPT_WRN, "WRN:test_0003: interrupted.\n");
        ret = -1;
    }
    else {

        snprintf(cmd, sizeof(cmd), "XPT_FLG=0x0003 %s 2>&1", command);

        fp = popen(cmd, "r");
        if (fp == NULL) {
            XPT(XPT_ERR, "ERR:test_0003: %s\n", command);
            ret = 1;
        }
        else {
	    int exp_idx = 0;
	    for (int i = 0; i < 32004; i++) {
                XPT(XPT_SNP, "SNP: ---< i = %d >---\n", i);
                if (!fgets(output, sizeof(output), fp)) {
                    XPT(XPT_ERR, "ERR:test_0003: 0 = fgets(fp)\n");
                    ret = 2;
                }
	        else {

		    switch (i) {
		    case 3:	// oasis_layer3 has key interrupt.
                    case 15999:
                    case 32002:
                    case 32003:
                       int len = strlen(expected_output[exp_idx]);
                       XPT(XPT_SNP, "SNP:test_0003: output = '%s'", output);
                       XPT(XPT_SNP, "SNP:test_0003: expected_output[exp_idx=%d] = '%s'\n", exp_idx, expected_output[exp_idx]);

		       if (strncmp(output, expected_output[exp_idx], len) != 0) {
			       XPT(XPT_ERR, "ERR:test_0003:line=%d: %s", i+1, output);
                               ret = 3;
		       }
		       else {
			       XPT(XPT_SNP, "SNP:test_0003:OK: %s\n", output);
			       exp_idx++;
		       }
		       XPT(XPT_SNP, "(4)\n");
		       break;

		    default:
			       XPT(XPT_SNP, "SNP:test_0003: %s\n", output);
		       break;
		    }
	        }
		if (ret) break;
	    }
            pclose(fp);
        }
    }

    XPT(XPT_SNP, "SNP:test_0003: ret = %d\n", ret);
    return ret;
}

int test_0004(void) {
    int ret = 0;
    const char *command="oasis_divs";
    FILE *fp;
    char output[1024];
    char cmd[512];
static const char *expected_output[] = { 
        "lcm(1,2)=2", 
        "lcm(1,2,3,...701)=2626719802592710061239991829174692172528343457131512513976281060940948726537757144360059535978552440933031373189919061646742591656110407680769241480651946904946211000952247368842571964792534944293239882868728171914661797572466549735970311937966946924655356399900939424721115438589874540587436119564591680000",
        "lcm(1,2,3,...1429)=201610960860866868929501122451843914738951266881474963717790224337916618173865310063771932785471271758560043375443071270524602158124610076478703204575840386097953849825277960294601930843596520281878356526468481479062915485922888764758008191396005171498320860382541582281718998071243029761270918288869268018176287154203546324923700127732399698127193381906737152540239879476474358141522956067184697190260263516776996987882199254795457837165240125419021146741188992064661089133450474716497223360446145019300218020421576913550036407219939062074151313212658790868830454822314446703804439375794046730869222931581893760000" };

    XPT(XPT_SNP, "SNP:test_0004: Start.\n");
    if (interrupted) {
        XPT(XPT_WRN, "WRN:test_0004: interrupted.\n");
        ret = -1;
    }
    else {

        snprintf(cmd, sizeof(cmd), "XPT_FLG=0x0003 %s 2>&1", command);

        fp = popen(cmd, "r");
        if (fp == NULL) {
            XPT(XPT_ERR, "ERR:test_0004: %s\n", command);
            ret = 1;
        }
        else {
	    int exp_idx = 0;
	    for (int i = 0; i < 180080; i++) {
                XPT(XPT_SNP, "SNP: ---< i = %d >---\n", i);
                if (!fgets(output, sizeof(output), fp)) {
                    XPT(XPT_ERR, "ERR:test_0004: 0 = fgets(fp)\n");
                    ret = 2;
                }
	        else {

		    switch (i) {
		    case 1:	// oasis_divs has no key interrupt.
                    case 49537:
                    case 180079:
                       int len = strlen(expected_output[exp_idx]);
                       XPT(XPT_SNP, "SNP:test_0004: output = '%s'", output);
                       XPT(XPT_SNP, "SNP:test_0004: expected_output[exp_idx=%d] = '%s'\n", exp_idx, expected_output[exp_idx]);

		       if (strncmp(output, expected_output[exp_idx], len) != 0) {
			       XPT(XPT_ERR, "ERR:test_0004:line=%d: %s", i+1, output);
                               ret = 3;
		       }
		       else {
			       XPT(XPT_SNP, "SNP:test_0004:OK: %s\n", output);
			       exp_idx++;
		       }
		       XPT(XPT_SNP, "(4)\n");
		       break;

		    default:
			       XPT(XPT_SNP, "SNP:test_0004: %s\n", output);
		       break;
		    }
	        }
		if (ret) break;
	    }
            pclose(fp);
        }
    }

    XPT(XPT_SNP, "SNP:test_0004: ret = %d\n", ret);
    return ret;
}

typedef struct {
    int number;
    const char *description;
    int (*func)(void);
} Test;

#if 0	// for only-one.
Test tests[] = {
    {6, "prime_oases:top-mid-bot-sta",  test_0006},
    {0, NULL, NULL}  // 終端マーカー
};
#else
Test tests[] = {
    {1, "oasis_layer1:top-mid-bot-sta", test_0001},
    {2, "oasis_layer2:top-mid-bot-sta", test_0002},
    {3, "oasis_layer3:top-mid-bot-sta", test_0003},
    {4, "oasis_divs:top-mid-bot",       test_0004},
    {5, "prime_oasis:top-mid-bot-sta",  test_0005},
    {6, "prime_oases:top-mid-bot-sta",  test_0006},
    {0, NULL, NULL}  // 終端マーカー
};
#endif

//int xpt_flg = XPT_ERR | XPT_WRN | XPT_SNP;
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
