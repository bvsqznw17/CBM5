#include "MachineParamList0.h"

class MParam0 : public MParam
{
public:
    int sn;
    const MachineParamList0::MParams &m;
    MParam0(MachineParamList0 *owner_, int sn_);
    virtual bool isValid() const;
    virtual int getAddr() const;
    virtual int getCount() const;
    virtual int getRDelay() const;
    virtual int getWDelay() const;
    virtual int getType() const;
    virtual int getPrec() const;
    virtual QString getUStr() const;
    virtual QString getRange() const;
    virtual QString vStr(const QVariant &value) const;
    virtual QVariant sVal(const QString &str) const;
};

MParam0::MParam0(MachineParamList0 *owner_, int sn_) :
    MParam(owner_),
    m(owner_->m)
{
    sn = sn_;
}

bool MParam0::isValid() const
{
    // auto code
    switch (sn) {
    case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:case 29:case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:case 41:case 42:case 43:case 44:case 45:case 46:case 47:case 48:case 49:case 50:case 51:case 52:case 53:case 54:case 55:case 56:case 57:case 58:case 59:case 60:case 61:case 62:case 63:case 64:case 65:case 66:case 67:case 68:case 69:case 70:case 71:case 72:case 73:case 74:case 75:case 76:case 77:case 78:case 79:case 80:case 81:case 82:case 83:case 84:case 85:case 86:case 87:case 88:case 89:case 90:case 91:case 92:case 93:case 94:case 95:case 96:case 97:case 98:case 99:case 100:case 101:case 102:case 103:case 104:case 105:case 106:case 107:case 108:case 109:case 110:case 111:case 112:case 113:case 114:case 115:case 116:case 117:case 118:case 119:case 120:case 121:case 122:case 123:case 124:case 125:case 126:case 127:case 128:case 129:case 130:case 131:case 132:case 133:case 134:case 135:case 136:case 137:case 138:case 139:case 140:case 141:case 142:case 143:case 144:case 145:case 146:case 147:case 148:case 149:case 150:case 151:case 152:case 153:case 154:case 155:case 156:case 157:case 158:case 159:case 160:case 161:case 162:case 163:case 164:case 165:case 166:case 167:case 168:case 169:case 170:case 171:case 172:case 173:case 174:case 175:case 176:case 177:case 178:case 179:case 180:case 181:case 182:case 183:case 184:case 185:case 186:case 187:case 188:case 189:case 190:case 191:case 192:case 193:case 194:case 195:case 196:case 197:case 198:case 199:case 200:case 201:case 202:case 203:case 204:case 205:case 206:case 207:case 208:case 209:case 210:case 211:case 212:case 213:case 214:case 215:case 216:case 217:case 218:case 219:case 220:case 221:case 222:case 223:case 224:case 225:case 226:case 227:case 228:case 229:case 230:case 231:case 232:case 233:case 234:case 235:case 236:case 237:case 238:case 239:case 240:case 241:case 242:case 243:case 244:case 245:case 246:case 247:case 248:case 249:case 250:case 251:case 252:case 253:case 254:case 255:case 256:case 257:case 258:case 259:case 260:case 261:case 262:case 263:case 264:case 265:case 266:case 267:case 268:case 269:case 270:case 271:case 272:case 273:case 274:case 275:case 276:case 277:case 278:case 279:case 280:case 281:case 282:case 283:case 284:case 285:case 286:case 287:case 288:case 289:case 290:case 291:case 292:case 293:case 294:case 295:case 296:case 297:case 298:case 299:case 300:case 301:case 302:case 303:case 304:case 305:case 306:case 307:case 308:case 309:case 310:case 311:case 312:case 313:case 314:case 315:case 316:case 317:case 318:case 319:case 320:case 321:case 322:case 323:case 324:case 325:case 326:case 327:case 328:case 329:case 330:case 331:case 332:case 333:case 334:case 335:case 336:case 337:case 338:case 339:case 340:case 341:case 342:case 343:case 344:case 345:case 346:case 347:case 348:case 349:case 350:case 351:case 352:case 353:case 354:case 355:case 356:case 357:case 358:case 359:case 360:case 361:case 362:case 363:case 364:case 365:case 366:case 367:case 368:case 369:case 370:case 371:case 372:case 373:case 374:case 375:case 376:case 377:case 378:case 379:case 380:case 381:case 382:case 383:case 384:case 385:case 386:case 387:case 388:case 389:case 390:case 391:case 392:case 393:case 394:case 395:case 396:case 397:case 398:case 399:case 400:case 401:case 402:case 403:case 404:case 405:case 406:case 407:case 408:case 409:case 410:case 411:case 412:case 413:case 414:case 415:case 416:case 417:case 418:case 419:case 420:case 421:case 422:case 423:case 424:case 425:case 426:case 427:case 428:case 429:case 430:case 431:case 432:case 433:case 434:case 435:case 436:case 437:case 438:case 439:case 440:case 441:case 442:case 443:case 444:case 445:case 446:case 447:case 448:case 449:case 450:case 451:case 452:case 453:case 454:case 455:case 456:case 457:case 458:case 459:case 460:case 461:case 462:case 463:case 464:case 465:case 466:case 467:case 468:case 469:case 470:case 471:case 472:case 473:case 474:case 475:case 476:case 477:case 478:case 479:case 480:case 481:case 482:case 483:case 484:case 485:case 486:case 487:case 488:case 489:case 490:case 491:case 492:case 493:case 494:case 495:case 496:case 497:case 498:case 499:case 500:case 501:case 502:case 503:case 504:case 505:case 506:case 507:case 508:case 509:case 510:case 511:case 512:case 513:case 514:case 515:case 516:case 517:case 518:case 519:case 520:case 521:case 522:case 523:case 524:case 525:case 526:case 527:case 528:case 529:case 530:case 531:case 532:case 533:case 534:case 535:case 536:case 537:case 538:case 539:case 540:case 541:case 542:case 543:case 544:case 545:case 546:case 547:case 548:case 549:case 550:case 551:case 552:case 553:case 554:case 555:case 556:case 557:case 558:case 559:case 560:case 561:case 562:case 563:case 564:case 565:case 566:case 567:case 568:case 569:case 570:case 571:case 572:case 573:case 574:case 575:case 576:case 577:case 578:case 579:case 580:case 581:case 582:case 583:case 584:case 585:case 586:case 587:case 588:case 589:case 590:case 591:case 592:case 593:case 594:case 595:case 596:case 597:case 598:case 599:case 600:case 601:case 602:case 603:case 604:case 605:case 606:case 607:case 608:case 609:case 610:case 611:case 612:case 613:case 614:case 615:case 616:case 617:case 618:case 619:case 620:case 621:case 622:case 623:case 624:case 625:case 626:case 627:case 628:case 629:case 630:case 631:case 632:case 633:case 634:case 635:case 636:case 637:case 638:case 639:case 640:case 641:case 642:case 643:case 644:case 645:case 646:case 647:case 648:case 649:case 650:case 651:case 652:case 653:case 654:case 655:
        return true;
    }
    return false;
}

int MParam0::getAddr() const
{
    // auto code
    switch (sn) {case 0:case 9:return 0;case 1:case 97:return 256;case 2:case 209:case 283:return 513;case 3:case 335:return 634;case 4:case 396:return 815;case 5:return 816;case 6:case 397:return 817;case 7:case 447:case 448:return 902;case 8:case 528:case 635:case 651:return 982;case 10:return 4;case 11:return 5;case 12:return 11;case 14:return 16;case 15:return 17;case 16:return 18;case 17:return 19;case 13:return 15;case 18:return 20;case 19:return 21;case 20:return 22;case 21:return 23;case 22:return 24;case 23:return 25;case 24:return 26;case 25:return 27;case 26:return 28;case 27:return 29;case 29:return 31;case 30:return 32;case 31:return 33;case 32:return 34;case 33:return 35;case 34:return 36;case 35:return 37;case 36:return 38;case 37:return 39;case 38:return 40;case 39:return 41;case 40:return 42;case 41:return 43;case 42:return 44;case 43:return 45;case 44:return 46;case 45:return 47;case 46:return 48;case 47:return 49;case 48:return 50;case 49:return 51;case 50:return 52;case 51:return 53;case 52:return 54;case 53:return 55;case 54:return 56;case 55:return 57;case 56:return 58;case 57:return 59;case 58:return 60;case 59:return 61;case 28:return 30;case 60:return 62;case 61:return 63;case 62:return 64;case 63:return 65;case 64:return 66;case 65:return 74;case 66:return 75;case 67:return 76;case 68:return 77;case 69:return 78;case 70:return 79;case 71:return 80;case 72:return 81;case 73:return 82;case 74:return 83;case 75:return 84;case 76:return 85;case 77:return 87;case 78:return 88;case 79:return 89;case 80:return 91;case 81:return 93;case 82:return 95;case 83:return 97;case 85:return 102;case 86:return 103;case 87:return 104;case 88:return 105;case 84:return 101;case 89:return 106;case 90:return 114;case 91:return 130;case 92:return 146;case 93:return 162;case 94:return 163;case 95:return 165;case 96:return 166;case 98:return 257;case 99:return 267;case 100:return 269;case 101:return 271;case 102:return 273;case 103:return 274;case 104:return 275;case 105:return 276;case 107:return 278;case 108:return 279;case 109:return 280;case 110:return 281;case 111:return 282;case 112:return 283;case 113:return 284;case 114:return 285;case 115:return 286;case 116:return 287;case 117:return 288;case 118:return 289;case 119:return 290;case 120:return 291;case 121:return 292;case 122:return 293;case 123:return 294;case 124:return 295;case 125:return 296;case 126:return 297;case 127:return 298;case 106:return 277;case 128:return 299;case 129:return 300;case 130:return 301;case 131:return 302;case 132:return 303;case 133:return 304;case 134:return 305;case 135:return 306;case 136:return 307;case 137:return 308;case 138:return 309;case 139:return 310;case 140:return 311;case 141:return 312;case 142:return 313;case 143:return 314;case 144:return 315;case 145:case 208:return 316;case 146:return 317;case 147:return 318;case 148:return 319;case 149:return 320;case 150:return 321;case 151:return 322;case 152:return 323;case 153:return 324;case 154:return 325;case 155:return 326;case 156:return 327;case 157:return 328;case 158:return 329;case 160:return 331;case 161:return 332;case 162:return 333;case 159:return 330;case 163:return 334;case 164:return 335;case 165:return 336;case 166:return 338;case 167:return 340;case 169:return 343;case 170:return 344;case 171:return 345;case 168:return 342;case 172:return 346;case 173:return 347;case 174:return 348;case 176:return 350;case 177:return 351;case 178:return 352;case 179:return 353;case 180:return 354;case 181:return 355;case 182:return 356;case 183:return 357;case 184:return 358;case 175:return 349;case 186:return 360;case 187:return 361;case 188:return 362;case 189:return 363;case 190:return 364;case 191:return 365;case 192:return 366;case 193:return 367;case 194:return 368;case 185:return 359;case 196:return 370;case 197:return 371;case 198:return 372;case 199:return 373;case 200:return 374;case 201:return 375;case 202:return 376;case 203:return 377;case 204:return 378;case 195:return 369;case 205:return 379;case 206:return 380;case 207:return 381;case 211:return 520;case 212:return 526;case 213:return 532;case 214:return 538;case 215:return 544;case 216:return 550;case 217:return 556;case 218:return 562;case 219:return 568;case 210:case 273:case 284:return 514;case 221:return 521;case 222:return 527;case 223:return 533;case 224:return 539;case 225:return 545;case 226:return 551;case 227:return 557;case 228:return 563;case 229:return 569;case 220:case 294:return 515;case 231:return 522;case 232:return 528;case 233:return 534;case 234:return 540;case 235:return 546;case 236:return 552;case 237:return 558;case 238:return 564;case 239:return 570;case 230:case 304:return 516;case 241:return 523;case 242:return 529;case 243:return 535;case 244:return 541;case 245:return 547;case 246:return 553;case 247:return 559;case 248:return 565;case 249:return 571;case 240:case 314:return 517;case 251:return 524;case 252:return 530;case 253:return 536;case 254:return 542;case 255:return 548;case 256:return 554;case 257:return 560;case 258:return 566;case 259:return 572;case 250:case 324:return 518;case 261:return 525;case 262:return 531;case 263:return 537;case 264:return 543;case 265:return 549;case 266:return 555;case 267:return 561;case 268:return 567;case 269:return 573;case 260:return 519;case 270:return 574;case 271:return 575;case 272:return 576;case 274:return 520;case 275:return 526;case 276:return 532;case 277:return 538;case 278:return 544;case 279:return 550;case 280:return 556;case 281:return 562;case 282:return 568;case 285:return 519;case 286:return 524;case 287:return 529;case 288:return 534;case 289:return 539;case 290:return 544;case 291:return 549;case 292:return 554;case 293:return 559;case 295:return 520;case 296:return 525;case 297:return 530;case 298:return 535;case 299:return 540;case 300:return 545;case 301:return 550;case 302:return 555;case 303:return 560;case 305:return 521;case 306:return 526;case 307:return 531;case 308:return 536;case 309:return 541;case 310:return 546;case 311:return 551;case 312:return 556;case 313:return 561;case 315:return 522;case 316:return 527;case 317:return 532;case 318:return 537;case 319:return 542;case 320:return 547;case 321:return 552;case 322:return 557;case 323:return 562;case 325:return 523;case 326:return 528;case 327:return 533;case 328:return 538;case 329:return 543;case 330:return 548;case 331:return 553;case 332:return 558;case 333:return 563;case 334:return 564;case 337:return 653;case 338:return 671;case 339:return 689;case 340:return 707;case 341:return 725;case 342:return 743;case 343:return 761;case 344:return 779;case 345:return 797;case 336:return 635;case 347:return 663;case 348:return 681;case 349:return 699;case 350:return 717;case 351:return 735;case 352:return 753;case 353:return 771;case 354:return 789;case 355:return 807;case 346:return 645;case 357:return 665;case 358:return 683;case 359:return 701;case 360:return 719;case 361:return 737;case 362:return 755;case 363:return 773;case 364:return 791;case 365:return 809;case 356:return 647;case 367:return 667;case 368:return 685;case 369:return 703;case 370:return 721;case 371:return 739;case 372:return 757;case 373:return 775;case 374:return 793;case 375:return 811;case 366:return 649;case 377:return 669;case 378:return 687;case 379:return 705;case 380:return 723;case 381:return 741;case 382:return 759;case 383:return 777;case 384:return 795;case 385:return 813;case 376:return 651;case 387:return 670;case 388:return 688;case 389:return 706;case 390:return 724;case 391:return 742;case 392:return 760;case 393:return 778;case 394:return 796;case 395:return 814;case 386:return 652;case 398:return 834;case 399:return 851;case 400:return 868;case 401:return 885;case 403:return 836;case 404:return 853;case 405:return 870;case 406:return 887;case 402:return 819;case 408:return 837;case 409:return 854;case 410:return 871;case 411:return 888;case 407:return 820;case 413:return 838;case 414:return 855;case 415:return 872;case 416:return 889;case 412:return 821;case 418:return 839;case 419:return 856;case 420:return 873;case 421:return 890;case 417:return 822;case 423:return 841;case 424:return 858;case 425:return 875;case 426:return 892;case 422:return 824;case 428:return 842;case 429:return 859;case 430:return 876;case 431:return 893;case 427:return 825;case 433:return 843;case 434:return 860;case 435:return 877;case 436:return 894;case 432:return 826;case 438:return 848;case 439:return 865;case 440:return 882;case 441:return 899;case 437:return 831;case 443:return 850;case 444:return 867;case 445:return 884;case 446:return 901;case 442:return 833;case 449:return 903;case 450:return 904;case 451:return 905;case 452:return 906;case 453:return 907;case 454:return 908;case 455:return 909;case 456:return 910;case 457:return 911;case 458:return 912;case 459:return 913;case 460:return 914;case 461:return 915;case 462:return 916;case 463:return 917;case 464:return 918;case 465:return 919;case 466:return 920;case 467:return 921;case 468:return 922;case 469:return 923;case 470:return 924;case 471:return 925;case 472:return 926;case 473:return 927;case 474:return 928;case 475:return 929;case 476:return 930;case 477:return 931;case 478:return 932;case 479:return 933;case 480:return 934;case 481:return 935;case 482:return 936;case 483:return 937;case 484:return 938;case 485:return 939;case 486:return 940;case 487:return 941;case 488:return 942;case 489:return 943;case 490:return 944;case 491:return 945;case 492:return 946;case 493:return 947;case 494:return 948;case 495:return 949;case 496:return 950;case 497:return 951;case 498:return 952;case 499:return 953;case 500:return 954;case 501:return 955;case 502:return 956;case 503:return 957;case 504:return 958;case 505:return 959;case 506:return 960;case 507:return 961;case 508:return 962;case 509:return 963;case 510:return 964;case 511:return 965;case 512:return 966;case 513:return 967;case 514:return 968;case 515:return 969;case 516:return 970;case 517:return 971;case 518:return 972;case 519:return 973;case 520:return 974;case 521:return 975;case 522:return 976;case 523:return 977;case 524:return 978;case 525:return 979;case 526:return 980;case 527:return 981;case 530:return 989;case 531:return 995;case 532:return 1001;case 533:return 1007;case 534:return 1013;case 535:return 1019;case 536:return 1025;case 537:return 1031;case 538:return 1037;case 539:return 1043;case 540:return 1049;case 541:return 1055;case 542:return 1061;case 543:return 1067;case 544:return 1073;case 545:return 1079;case 546:return 1085;case 547:return 1091;case 548:return 1097;case 549:return 1103;case 550:return 1109;case 551:return 1115;case 552:return 1121;case 553:return 1127;case 554:return 1133;case 555:return 1139;case 556:return 1145;case 557:return 1151;case 558:return 1157;case 559:return 1163;case 560:return 1169;case 561:return 1175;case 562:return 1181;case 563:return 1187;case 529:case 636:case 652:return 983;case 565:return 993;case 566:return 999;case 567:return 1005;case 568:return 1011;case 569:return 1017;case 570:return 1023;case 571:return 1029;case 572:return 1035;case 573:return 1041;case 574:return 1047;case 575:return 1053;case 576:return 1059;case 577:return 1065;case 578:return 1071;case 579:return 1077;case 580:return 1083;case 581:return 1089;case 582:return 1095;case 583:return 1101;case 584:return 1107;case 585:return 1113;case 586:return 1119;case 587:return 1125;case 588:return 1131;case 589:return 1137;case 590:return 1143;case 591:return 1149;case 592:return 1155;case 593:return 1161;case 594:return 1167;case 595:return 1173;case 596:return 1179;case 597:return 1185;case 598:return 1191;case 564:case 640:return 987;case 600:return 994;case 601:return 1000;case 602:return 1006;case 603:return 1012;case 604:return 1018;case 605:return 1024;case 606:return 1030;case 607:return 1036;case 608:return 1042;case 609:return 1048;case 610:return 1054;case 611:return 1060;case 612:return 1066;case 613:return 1072;case 614:return 1078;case 615:return 1084;case 616:return 1090;case 617:return 1096;case 618:return 1102;case 619:return 1108;case 620:return 1114;case 621:return 1120;case 622:return 1126;case 623:return 1132;case 624:return 1138;case 625:return 1144;case 626:return 1150;case 627:return 1156;case 628:return 1162;case 629:return 1168;case 630:return 1174;case 631:return 1180;case 632:return 1186;case 633:return 1192;case 599:return 988;case 634:return 1193;case 637:case 653:return 984;case 638:case 654:return 985;case 639:return 986;case 641:return 1087;case 642:return 1187;case 643:return 1287;case 644:return 1387;case 645:return 1487;case 646:return 1587;case 647:return 1687;case 648:return 1787;case 649:return 1887;case 650:return 1987;case 655:return 2006;
    }
    return 0;
}

int MParam0::getCount() const
{
    // auto code
    switch (sn) {
    case 0:
        return 256;
    case 1:
        return 257;
    case 2:
        return 121;
    case 3:
        return 181;
    case 4:case 5:case 10:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:case 29:case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:case 41:case 42:case 43:case 44:case 45:case 46:case 47:case 48:case 49:case 50:case 51:case 52:case 53:case 54:case 55:case 56:case 57:case 58:case 59:case 60:case 61:case 62:case 63:case 65:case 66:case 67:case 68:case 69:case 70:case 71:case 72:case 73:case 74:case 75:case 77:case 78:case 84:case 85:case 86:case 87:case 88:case 93:case 95:case 97:case 102:case 103:case 104:case 105:case 106:case 107:case 108:case 109:case 110:case 111:case 112:case 113:case 114:case 115:case 116:case 117:case 118:case 119:case 120:case 121:case 122:case 123:case 124:case 125:case 126:case 127:case 128:case 129:case 130:case 131:case 132:case 133:case 134:case 135:case 136:case 137:case 138:case 139:case 140:case 141:case 142:case 143:case 144:case 145:case 146:case 147:case 148:case 149:case 150:case 151:case 152:case 153:case 154:case 155:case 156:case 157:case 158:case 159:case 160:case 161:case 162:case 163:case 164:case 168:case 169:case 170:case 171:case 172:case 173:case 174:case 175:case 176:case 177:case 178:case 179:case 180:case 181:case 182:case 183:case 184:case 185:case 186:case 187:case 188:case 189:case 190:case 191:case 192:case 193:case 194:case 195:case 196:case 197:case 198:case 199:case 200:case 201:case 202:case 203:case 204:case 205:case 206:case 208:case 209:case 210:case 211:case 212:case 213:case 214:case 215:case 216:case 217:case 218:case 219:case 220:case 221:case 222:case 223:case 224:case 225:case 226:case 227:case 228:case 229:case 230:case 231:case 232:case 233:case 234:case 235:case 236:case 237:case 238:case 239:case 240:case 241:case 242:case 243:case 244:case 245:case 246:case 247:case 248:case 249:case 250:case 251:case 252:case 253:case 254:case 255:case 256:case 257:case 258:case 259:case 260:case 261:case 262:case 263:case 264:case 265:case 266:case 267:case 268:case 269:case 270:case 271:case 273:case 274:case 275:case 276:case 277:case 278:case 279:case 280:case 281:case 282:case 283:case 284:case 285:case 286:case 287:case 288:case 289:case 290:case 291:case 292:case 293:case 294:case 295:case 296:case 297:case 298:case 299:case 300:case 301:case 302:case 303:case 304:case 305:case 306:case 307:case 308:case 309:case 310:case 311:case 312:case 313:case 314:case 315:case 316:case 317:case 318:case 319:case 320:case 321:case 322:case 323:case 324:case 325:case 326:case 327:case 328:case 329:case 330:case 331:case 332:case 333:case 335:case 376:case 377:case 378:case 379:case 380:case 381:case 382:case 383:case 384:case 385:case 386:case 387:case 388:case 389:case 390:case 391:case 392:case 393:case 394:case 395:case 402:case 403:case 404:case 405:case 406:case 407:case 408:case 409:case 410:case 411:case 412:case 413:case 414:case 415:case 416:case 422:case 423:case 424:case 425:case 426:case 427:case 428:case 429:case 430:case 431:case 442:case 443:case 444:case 445:case 446:case 448:case 449:case 450:case 451:case 452:case 453:case 454:case 455:case 456:case 457:case 458:case 459:case 460:case 461:case 462:case 463:case 464:case 465:case 466:case 467:case 468:case 469:case 470:case 471:case 472:case 473:case 474:case 475:case 476:case 477:case 478:case 479:case 480:case 481:case 482:case 483:case 484:case 485:case 486:case 487:case 488:case 489:case 490:case 491:case 492:case 493:case 494:case 495:case 496:case 497:case 498:case 499:case 500:case 501:case 502:case 503:case 504:case 505:case 506:case 507:case 508:case 509:case 510:case 511:case 512:case 513:case 514:case 515:case 516:case 517:case 518:case 519:case 520:case 521:case 522:case 523:case 524:case 525:case 526:case 527:case 528:case 564:case 565:case 566:case 567:case 568:case 569:case 570:case 571:case 572:case 573:case 574:case 575:case 576:case 577:case 578:case 579:case 580:case 581:case 582:case 583:case 584:case 585:case 586:case 587:case 588:case 589:case 590:case 591:case 592:case 593:case 594:case 595:case 596:case 597:case 598:case 599:case 600:case 601:case 602:case 603:case 604:case 605:case 606:case 607:case 608:case 609:case 610:case 611:case 612:case 613:case 614:case 615:case 616:case 617:case 618:case 619:case 620:case 621:case 622:case 623:case 624:case 625:case 626:case 627:case 628:case 629:case 630:case 631:case 632:case 633:case 635:case 636:case 637:case 638:case 639:case 651:case 652:case 653:
        return 1;
    case 6:
        return 85;
    case 7:
        return 80;
    case 8:
        return 1024;
    case 9:case 12:case 83:case 529:case 530:case 531:case 532:case 533:case 534:case 535:case 536:case 537:case 538:case 539:case 540:case 541:case 542:case 543:case 544:case 545:case 546:case 547:case 548:case 549:case 550:case 551:case 552:case 553:case 554:case 555:case 556:case 557:case 558:case 559:case 560:case 561:case 562:case 563:
        return 4;
    case 11:
        return 6;
    case 64:case 89:
        return 8;
    case 76:case 79:case 80:case 81:case 82:case 94:case 99:case 100:case 101:case 165:case 166:case 167:case 346:case 347:case 348:case 349:case 350:case 351:case 352:case 353:case 354:case 355:case 356:case 357:case 358:case 359:case 360:case 361:case 362:case 363:case 364:case 365:case 366:case 367:case 368:case 369:case 370:case 371:case 372:case 373:case 374:case 375:case 397:case 398:case 399:case 400:case 401:case 417:case 418:case 419:case 420:case 421:case 437:case 438:case 439:case 440:case 441:
        return 2;
    case 90:case 91:case 92:
        return 16;
    case 96:case 207:case 272:case 334:case 396:case 447:case 634:case 650:case 654:case 655:
        return 0;
    case 98:case 336:case 337:case 338:case 339:case 340:case 341:case 342:case 343:case 344:case 345:
        return 10;
    case 432:case 433:case 434:case 435:case 436:
        return 5;
    case 640:case 641:case 642:case 643:case 644:case 645:case 646:case 647:case 648:case 649:
        return 100;
    }
    return 0;
}

int MParam0::getRDelay() const
{
    // auto code
    switch (sn) {
    case 0:case 1:case 2:case 3:case 6:case 7:case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:case 29:case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:case 41:case 42:case 43:case 44:case 45:case 46:case 47:case 48:case 49:case 50:case 51:case 52:case 53:case 54:case 55:case 56:case 57:case 58:case 59:case 60:case 61:case 62:case 63:case 64:case 65:case 66:case 67:case 68:case 69:case 70:case 71:case 72:case 73:case 74:case 75:case 76:case 77:case 78:case 79:case 80:case 81:case 82:case 83:case 84:case 85:case 86:case 87:case 88:case 89:case 90:case 91:case 92:case 93:case 94:case 95:case 96:case 97:case 98:case 99:case 100:case 101:case 102:case 103:case 104:case 105:case 106:case 107:case 108:case 109:case 110:case 111:case 112:case 113:case 114:case 115:case 116:case 117:case 118:case 119:case 120:case 121:case 122:case 123:case 124:case 125:case 126:case 127:case 128:case 129:case 130:case 131:case 132:case 133:case 134:case 135:case 136:case 137:case 138:case 139:case 140:case 141:case 142:case 143:case 144:case 145:case 146:case 147:case 148:case 149:case 150:case 151:case 152:case 153:case 154:case 155:case 156:case 157:case 158:case 159:case 160:case 161:case 162:case 163:case 164:case 165:case 166:case 167:case 168:case 169:case 170:case 171:case 172:case 173:case 174:case 175:case 176:case 177:case 178:case 179:case 180:case 181:case 182:case 183:case 184:case 185:case 186:case 187:case 188:case 189:case 190:case 191:case 192:case 193:case 194:case 195:case 196:case 197:case 198:case 199:case 200:case 201:case 202:case 203:case 204:case 205:case 206:case 207:case 208:case 209:case 210:case 211:case 212:case 213:case 214:case 215:case 216:case 217:case 218:case 219:case 220:case 221:case 222:case 223:case 224:case 225:case 226:case 227:case 228:case 229:case 230:case 231:case 232:case 233:case 234:case 235:case 236:case 237:case 238:case 239:case 240:case 241:case 242:case 243:case 244:case 245:case 246:case 247:case 248:case 249:case 250:case 251:case 252:case 253:case 254:case 255:case 256:case 257:case 258:case 259:case 260:case 261:case 262:case 263:case 264:case 265:case 266:case 267:case 268:case 269:case 270:case 271:case 272:case 273:case 274:case 275:case 276:case 277:case 278:case 279:case 280:case 281:case 282:case 283:case 284:case 285:case 286:case 287:case 288:case 289:case 290:case 291:case 292:case 293:case 294:case 295:case 296:case 297:case 298:case 299:case 300:case 301:case 302:case 303:case 304:case 305:case 306:case 307:case 308:case 309:case 310:case 311:case 312:case 313:case 314:case 315:case 316:case 317:case 318:case 319:case 320:case 321:case 322:case 323:case 324:case 325:case 326:case 327:case 328:case 329:case 330:case 331:case 332:case 333:case 334:case 335:case 336:case 337:case 338:case 339:case 340:case 341:case 342:case 343:case 344:case 345:case 346:case 347:case 348:case 349:case 350:case 351:case 352:case 353:case 354:case 355:case 356:case 357:case 358:case 359:case 360:case 361:case 362:case 363:case 364:case 365:case 366:case 367:case 368:case 369:case 370:case 371:case 372:case 373:case 374:case 375:case 376:case 377:case 378:case 379:case 380:case 381:case 382:case 383:case 384:case 385:case 386:case 387:case 388:case 389:case 390:case 391:case 392:case 393:case 394:case 395:case 396:case 397:case 398:case 399:case 400:case 401:case 402:case 403:case 404:case 405:case 406:case 407:case 408:case 409:case 410:case 411:case 412:case 413:case 414:case 415:case 416:case 417:case 418:case 419:case 420:case 421:case 422:case 423:case 424:case 425:case 426:case 427:case 428:case 429:case 430:case 431:case 432:case 433:case 434:case 435:case 436:case 437:case 438:case 439:case 440:case 441:case 442:case 443:case 444:case 445:case 446:case 447:case 448:case 449:case 450:case 451:case 452:case 453:case 454:case 455:case 456:case 457:case 458:case 459:case 460:case 461:case 462:case 463:case 464:case 465:case 466:case 467:case 468:case 469:case 470:case 471:case 472:case 473:case 474:case 475:case 476:case 477:case 478:case 479:case 480:case 481:case 482:case 483:case 484:case 485:case 486:case 487:case 488:case 489:case 490:case 491:case 492:case 493:case 494:case 495:case 496:case 497:case 498:case 499:case 500:case 501:case 502:case 503:case 504:case 505:case 506:case 507:case 508:case 509:case 510:case 511:case 512:case 513:case 514:case 515:case 516:case 517:case 518:case 519:case 520:case 521:case 522:case 523:case 524:case 525:case 526:case 527:case 528:case 529:case 530:case 531:case 532:case 533:case 534:case 535:case 536:case 537:case 538:case 539:case 540:case 541:case 542:case 543:case 544:case 545:case 546:case 547:case 548:case 549:case 550:case 551:case 552:case 553:case 554:case 555:case 556:case 557:case 558:case 559:case 560:case 561:case 562:case 563:case 564:case 565:case 566:case 567:case 568:case 569:case 570:case 571:case 572:case 573:case 574:case 575:case 576:case 577:case 578:case 579:case 580:case 581:case 582:case 583:case 584:case 585:case 586:case 587:case 588:case 589:case 590:case 591:case 592:case 593:case 594:case 595:case 596:case 597:case 598:case 599:case 600:case 601:case 602:case 603:case 604:case 605:case 606:case 607:case 608:case 609:case 610:case 611:case 612:case 613:case 614:case 615:case 616:case 617:case 618:case 619:case 620:case 621:case 622:case 623:case 624:case 625:case 626:case 627:case 628:case 629:case 630:case 631:case 632:case 633:case 634:case 635:case 636:case 637:case 638:case 639:case 640:case 641:case 642:case 643:case 644:case 645:case 646:case 647:case 648:case 649:case 650:case 651:case 652:case 653:case 654:case 655:
        return 200;
    case 4:case 5:
        return 3000;
    }
    return 0;
}

int MParam0::getWDelay() const
{
    // auto code
    switch (sn) {
    case 0:case 1:case 2:case 3:case 6:case 7:case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:case 29:case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:case 41:case 42:case 43:case 44:case 45:case 46:case 47:case 48:case 49:case 50:case 51:case 52:case 53:case 54:case 55:case 56:case 57:case 58:case 59:case 60:case 61:case 62:case 63:case 64:case 65:case 66:case 67:case 68:case 69:case 70:case 71:case 72:case 73:case 74:case 75:case 76:case 77:case 78:case 79:case 80:case 81:case 82:case 83:case 84:case 85:case 86:case 87:case 88:case 89:case 90:case 91:case 92:case 93:case 94:case 95:case 96:case 97:case 98:case 99:case 100:case 101:case 102:case 103:case 104:case 105:case 106:case 107:case 108:case 109:case 110:case 111:case 112:case 113:case 114:case 115:case 116:case 117:case 118:case 119:case 120:case 121:case 122:case 123:case 124:case 125:case 126:case 127:case 128:case 129:case 130:case 131:case 132:case 133:case 134:case 135:case 136:case 137:case 138:case 139:case 140:case 141:case 142:case 143:case 144:case 145:case 146:case 147:case 148:case 149:case 150:case 151:case 152:case 153:case 154:case 155:case 156:case 157:case 158:case 159:case 160:case 161:case 162:case 163:case 164:case 165:case 166:case 167:case 168:case 169:case 170:case 171:case 172:case 173:case 174:case 175:case 176:case 177:case 178:case 179:case 180:case 181:case 182:case 183:case 184:case 185:case 186:case 187:case 188:case 189:case 190:case 191:case 192:case 193:case 194:case 195:case 196:case 197:case 198:case 199:case 200:case 201:case 202:case 203:case 204:case 205:case 206:case 207:case 208:case 209:case 210:case 211:case 212:case 213:case 214:case 215:case 216:case 217:case 218:case 219:case 220:case 221:case 222:case 223:case 224:case 225:case 226:case 227:case 228:case 229:case 230:case 231:case 232:case 233:case 234:case 235:case 236:case 237:case 238:case 239:case 240:case 241:case 242:case 243:case 244:case 245:case 246:case 247:case 248:case 249:case 250:case 251:case 252:case 253:case 254:case 255:case 256:case 257:case 258:case 259:case 260:case 261:case 262:case 263:case 264:case 265:case 266:case 267:case 268:case 269:case 270:case 271:case 272:case 273:case 274:case 275:case 276:case 277:case 278:case 279:case 280:case 281:case 282:case 283:case 284:case 285:case 286:case 287:case 288:case 289:case 290:case 291:case 292:case 293:case 294:case 295:case 296:case 297:case 298:case 299:case 300:case 301:case 302:case 303:case 304:case 305:case 306:case 307:case 308:case 309:case 310:case 311:case 312:case 313:case 314:case 315:case 316:case 317:case 318:case 319:case 320:case 321:case 322:case 323:case 324:case 325:case 326:case 327:case 328:case 329:case 330:case 331:case 332:case 333:case 334:case 335:case 336:case 337:case 338:case 339:case 340:case 341:case 342:case 343:case 344:case 345:case 346:case 347:case 348:case 349:case 350:case 351:case 352:case 353:case 354:case 355:case 356:case 357:case 358:case 359:case 360:case 361:case 362:case 363:case 364:case 365:case 366:case 367:case 368:case 369:case 370:case 371:case 372:case 373:case 374:case 375:case 376:case 377:case 378:case 379:case 380:case 381:case 382:case 383:case 384:case 385:case 386:case 387:case 388:case 389:case 390:case 391:case 392:case 393:case 394:case 395:case 396:case 397:case 398:case 399:case 400:case 401:case 402:case 403:case 404:case 405:case 406:case 407:case 408:case 409:case 410:case 411:case 412:case 413:case 414:case 415:case 416:case 417:case 418:case 419:case 420:case 421:case 422:case 423:case 424:case 425:case 426:case 427:case 428:case 429:case 430:case 431:case 432:case 433:case 434:case 435:case 436:case 437:case 438:case 439:case 440:case 441:case 442:case 443:case 444:case 445:case 446:case 447:case 448:case 449:case 450:case 451:case 452:case 453:case 454:case 455:case 456:case 457:case 458:case 459:case 460:case 461:case 462:case 463:case 464:case 465:case 466:case 467:case 468:case 469:case 470:case 471:case 472:case 473:case 474:case 475:case 476:case 477:case 478:case 479:case 480:case 481:case 482:case 483:case 484:case 485:case 486:case 487:case 488:case 489:case 490:case 491:case 492:case 493:case 494:case 495:case 496:case 497:case 498:case 499:case 500:case 501:case 502:case 503:case 504:case 505:case 506:case 507:case 508:case 509:case 510:case 511:case 512:case 513:case 514:case 515:case 516:case 517:case 518:case 519:case 520:case 521:case 522:case 523:case 524:case 525:case 526:case 527:case 528:case 529:case 530:case 531:case 532:case 533:case 534:case 535:case 536:case 537:case 538:case 539:case 540:case 541:case 542:case 543:case 544:case 545:case 546:case 547:case 548:case 549:case 550:case 551:case 552:case 553:case 554:case 555:case 556:case 557:case 558:case 559:case 560:case 561:case 562:case 563:case 564:case 565:case 566:case 567:case 568:case 569:case 570:case 571:case 572:case 573:case 574:case 575:case 576:case 577:case 578:case 579:case 580:case 581:case 582:case 583:case 584:case 585:case 586:case 587:case 588:case 589:case 590:case 591:case 592:case 593:case 594:case 595:case 596:case 597:case 598:case 599:case 600:case 601:case 602:case 603:case 604:case 605:case 606:case 607:case 608:case 609:case 610:case 611:case 612:case 613:case 614:case 615:case 616:case 617:case 618:case 619:case 620:case 621:case 622:case 623:case 624:case 625:case 626:case 627:case 628:case 629:case 630:case 631:case 632:case 633:case 634:case 635:case 636:case 637:case 638:case 639:case 640:case 641:case 642:case 643:case 644:case 645:case 646:case 647:case 648:case 649:case 650:case 651:case 652:case 653:case 654:case 655:
        return 200;
    case 4:case 5:
        return 3000;
    }
    return 0;
}

int MParam0::getType() const
{
    // auto code
    switch (sn) {
    case 0:case 1:case 2:case 3:case 6:case 7:case 8:case 9:case 12:case 64:case 83:case 89:case 90:case 91:case 92:case 96:case 207:case 272:case 334:case 396:case 432:case 433:case 434:case 435:case 436:case 447:case 634:case 650:case 654:case 655:
        return Bytes;
    case 4:case 5:case 10:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:case 29:case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:case 41:case 42:case 43:case 44:case 45:case 46:case 47:case 48:case 49:case 50:case 51:case 52:case 53:case 54:case 55:case 56:case 57:case 58:case 59:case 60:case 61:case 62:case 63:case 65:case 66:case 67:case 68:case 69:case 70:case 71:case 72:case 73:case 74:case 75:case 77:case 78:case 93:case 95:case 97:case 102:case 103:case 105:case 106:case 107:case 108:case 109:case 110:case 111:case 112:case 113:case 114:case 115:case 116:case 117:case 118:case 119:case 120:case 121:case 122:case 123:case 124:case 125:case 126:case 127:case 130:case 131:case 132:case 133:case 134:case 137:case 138:case 139:case 142:case 143:case 144:case 145:case 146:case 147:case 148:case 149:case 150:case 151:case 152:case 154:case 155:case 156:case 157:case 158:case 159:case 160:case 161:case 162:case 163:case 164:case 168:case 169:case 170:case 171:case 172:case 173:case 174:case 175:case 176:case 177:case 178:case 179:case 180:case 181:case 182:case 183:case 184:case 205:case 206:case 208:case 209:case 220:case 221:case 222:case 223:case 224:case 225:case 226:case 227:case 228:case 229:case 230:case 231:case 232:case 233:case 234:case 235:case 236:case 237:case 238:case 239:case 240:case 241:case 242:case 243:case 244:case 245:case 246:case 247:case 248:case 249:case 250:case 251:case 252:case 253:case 254:case 255:case 256:case 257:case 258:case 259:case 260:case 261:case 262:case 263:case 264:case 265:case 266:case 267:case 268:case 269:case 283:case 294:case 295:case 296:case 297:case 298:case 299:case 300:case 301:case 302:case 303:case 304:case 305:case 306:case 307:case 308:case 309:case 310:case 311:case 312:case 313:case 314:case 315:case 316:case 317:case 318:case 319:case 320:case 321:case 322:case 323:case 324:case 325:case 326:case 327:case 328:case 329:case 330:case 331:case 332:case 333:case 335:case 376:case 377:case 378:case 379:case 380:case 381:case 382:case 383:case 384:case 385:case 386:case 387:case 388:case 389:case 390:case 391:case 392:case 393:case 394:case 395:case 402:case 403:case 404:case 405:case 406:case 407:case 408:case 409:case 410:case 411:case 412:case 413:case 414:case 415:case 416:case 422:case 423:case 424:case 425:case 426:case 442:case 443:case 444:case 445:case 446:case 528:case 564:case 565:case 566:case 567:case 568:case 569:case 570:case 571:case 572:case 573:case 574:case 575:case 576:case 577:case 578:case 579:case 580:case 581:case 582:case 583:case 584:case 585:case 586:case 587:case 588:case 589:case 590:case 591:case 592:case 593:case 594:case 595:case 596:case 597:case 598:case 599:case 600:case 601:case 602:case 603:case 604:case 605:case 606:case 607:case 608:case 609:case 610:case 611:case 612:case 613:case 614:case 615:case 616:case 617:case 618:case 619:case 620:case 621:case 622:case 623:case 624:case 625:case 626:case 627:case 628:case 629:case 630:case 631:case 632:case 633:case 635:case 636:case 639:case 653:
        return U16;
    case 11:case 98:case 336:case 337:case 338:case 339:case 340:case 341:case 342:case 343:case 344:case 345:case 529:case 530:case 531:case 532:case 533:case 534:case 535:case 536:case 537:case 538:case 539:case 540:case 541:case 542:case 543:case 544:case 545:case 546:case 547:case 548:case 549:case 550:case 551:case 552:case 553:case 554:case 555:case 556:case 557:case 558:case 559:case 560:case 561:case 562:case 563:
        return Chars;
    case 76:case 99:case 100:case 101:case 165:case 166:case 167:case 346:case 347:case 348:case 349:case 350:case 351:case 352:case 353:case 354:case 355:case 356:case 357:case 358:case 359:case 360:case 361:case 362:case 363:case 364:case 365:case 366:case 367:case 368:case 369:case 370:case 371:case 372:case 373:case 374:case 375:case 397:case 398:case 399:case 400:case 401:case 437:case 438:case 439:case 440:case 441:
        return S32;
    case 79:case 80:case 81:case 82:case 84:case 85:case 86:case 87:case 88:case 94:case 417:case 418:case 419:case 420:case 421:
        return U32;
    case 104:case 128:case 129:case 135:case 136:case 140:case 141:case 153:case 185:case 186:case 187:case 188:case 189:case 190:case 191:case 192:case 193:case 194:case 195:case 196:case 197:case 198:case 199:case 200:case 201:case 202:case 203:case 204:case 210:case 211:case 212:case 213:case 214:case 215:case 216:case 217:case 218:case 219:case 270:case 271:case 273:case 274:case 275:case 276:case 277:case 278:case 279:case 280:case 281:case 282:case 284:case 285:case 286:case 287:case 288:case 289:case 290:case 291:case 292:case 293:case 427:case 428:case 429:case 430:case 431:case 448:case 449:case 450:case 451:case 452:case 453:case 454:case 455:case 456:case 457:case 458:case 459:case 460:case 461:case 462:case 463:case 464:case 465:case 466:case 467:case 468:case 469:case 470:case 471:case 472:case 473:case 474:case 475:case 476:case 477:case 478:case 479:case 480:case 481:case 482:case 483:case 484:case 485:case 486:case 487:case 488:case 489:case 490:case 491:case 492:case 493:case 494:case 495:case 496:case 497:case 498:case 499:case 500:case 501:case 502:case 503:case 504:case 505:case 506:case 507:case 508:case 509:case 510:case 511:case 512:case 513:case 514:case 515:case 516:case 517:case 518:case 519:case 520:case 521:case 522:case 523:case 524:case 525:case 526:case 527:case 637:case 638:case 651:case 652:
        return S16;
    case 640:case 641:case 642:case 643:case 644:case 645:case 646:case 647:case 648:case 649:
        return S16s;
    }
    return 0;
}

int MParam0::getPrec() const
{
    // auto code
    switch (sn) {
    case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:case 29:case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:case 41:case 42:case 43:case 44:case 45:case 46:case 47:case 48:case 49:case 50:case 51:case 52:case 53:case 54:case 55:case 56:case 57:case 58:case 59:case 63:case 64:case 65:case 66:case 67:case 68:case 69:case 70:case 71:case 72:case 73:case 74:case 75:case 77:case 83:case 84:case 85:case 86:case 87:case 88:case 89:case 90:case 91:case 92:case 93:case 94:case 95:case 96:case 97:case 98:case 102:case 103:case 105:case 106:case 107:case 108:case 109:case 110:case 111:case 112:case 113:case 114:case 115:case 116:case 117:case 118:case 119:case 120:case 121:case 122:case 123:case 124:case 125:case 126:case 127:case 130:case 131:case 132:case 134:case 135:case 137:case 139:case 142:case 143:case 144:case 145:case 146:case 147:case 148:case 149:case 150:case 151:case 152:case 153:case 154:case 155:case 156:case 157:case 158:case 159:case 160:case 161:case 162:case 164:case 168:case 169:case 170:case 171:case 172:case 173:case 174:case 175:case 176:case 177:case 178:case 179:case 180:case 181:case 182:case 183:case 184:case 205:case 206:case 207:case 208:case 209:case 220:case 221:case 222:case 223:case 224:case 225:case 226:case 227:case 228:case 229:case 230:case 231:case 232:case 233:case 234:case 235:case 236:case 237:case 238:case 239:case 240:case 241:case 242:case 243:case 244:case 245:case 246:case 247:case 248:case 249:case 250:case 251:case 252:case 253:case 254:case 255:case 256:case 257:case 258:case 259:case 260:case 261:case 262:case 263:case 264:case 265:case 266:case 267:case 268:case 269:case 272:case 273:case 274:case 275:case 276:case 277:case 278:case 279:case 280:case 281:case 282:case 283:case 284:case 285:case 286:case 287:case 288:case 289:case 290:case 291:case 292:case 293:case 294:case 295:case 296:case 297:case 298:case 299:case 300:case 301:case 302:case 303:case 304:case 305:case 306:case 307:case 308:case 309:case 310:case 311:case 312:case 313:case 314:case 315:case 316:case 317:case 318:case 319:case 320:case 321:case 322:case 323:case 324:case 325:case 326:case 327:case 328:case 329:case 330:case 331:case 332:case 333:case 334:case 335:case 336:case 337:case 338:case 339:case 340:case 341:case 342:case 343:case 344:case 345:case 346:case 347:case 348:case 349:case 350:case 351:case 352:case 353:case 354:case 355:case 356:case 357:case 358:case 359:case 360:case 361:case 362:case 363:case 364:case 365:case 366:case 367:case 368:case 369:case 370:case 371:case 372:case 373:case 374:case 375:case 376:case 377:case 378:case 379:case 380:case 381:case 382:case 383:case 384:case 385:case 386:case 387:case 388:case 389:case 390:case 391:case 392:case 393:case 394:case 395:case 396:case 402:case 403:case 404:case 405:case 406:case 407:case 408:case 409:case 410:case 411:case 412:case 413:case 414:case 415:case 416:case 417:case 418:case 419:case 420:case 421:case 422:case 423:case 424:case 425:case 426:case 432:case 433:case 434:case 435:case 436:case 442:case 443:case 444:case 445:case 446:case 447:case 528:case 529:case 530:case 531:case 532:case 533:case 534:case 535:case 536:case 537:case 538:case 539:case 540:case 541:case 542:case 543:case 544:case 545:case 546:case 547:case 548:case 549:case 550:case 551:case 552:case 553:case 554:case 555:case 556:case 557:case 558:case 559:case 560:case 561:case 562:case 563:case 564:case 565:case 566:case 567:case 568:case 569:case 570:case 571:case 572:case 573:case 574:case 575:case 576:case 577:case 578:case 579:case 580:case 581:case 582:case 583:case 584:case 585:case 586:case 587:case 588:case 589:case 590:case 591:case 592:case 593:case 594:case 595:case 596:case 597:case 598:case 599:case 600:case 601:case 602:case 603:case 604:case 605:case 606:case 607:case 608:case 609:case 610:case 611:case 612:case 613:case 614:case 615:case 616:case 617:case 618:case 619:case 620:case 621:case 622:case 623:case 624:case 625:case 626:case 627:case 628:case 629:case 630:case 631:case 632:case 633:case 634:case 635:case 636:case 639:case 640:case 641:case 642:case 643:case 644:case 645:case 646:case 647:case 648:case 649:case 650:case 653:case 654:case 655:
        return 0;
    case 60:case 76:case 79:case 81:case 128:case 129:case 136:case 138:case 140:case 163:case 185:case 186:case 187:case 188:case 189:case 190:case 191:case 192:case 193:case 194:case 195:case 196:case 197:case 198:case 199:case 200:case 201:case 202:case 203:case 204:case 427:case 428:case 429:case 430:case 431:case 448:case 449:case 450:case 451:case 452:case 453:case 454:case 455:case 456:case 457:case 458:case 459:case 460:case 461:case 462:case 463:case 464:case 465:case 466:case 467:case 468:case 469:case 470:case 471:case 472:case 473:case 474:case 475:case 476:case 477:case 478:case 479:case 480:case 481:case 482:case 483:case 484:case 485:case 486:case 487:case 488:case 489:case 490:case 491:case 492:case 493:case 494:case 495:case 496:case 497:case 498:case 499:case 500:case 501:case 502:case 503:case 504:case 505:case 506:case 507:case 508:case 509:case 510:case 511:case 512:case 513:case 514:case 515:case 516:case 517:case 518:case 519:case 520:case 521:case 522:case 523:case 524:case 525:case 526:case 527:case 637:case 638:case 651:case 652:
        {
        /*系统小数位*/
        return m.sys_dot_num->value().toInt() % 4;
        }
        break;
    case 61:case 62:
        {
        /*振动小数点位数与其量纲关系：2,2,2,1*/
        switch (m.sys_dot_num->value().toInt()) {
        case 0: case 1: return 2;
        case 2: return 2;
        case 3: return 1;
        }
        }
        break;
    case 78:
        return 3;
    case 80:case 82:case 165:case 166:
        {
        /*主振小数点位数与其量纲关系：2,2,3,1*/
        switch (m.sys_dot_num->value().toInt()) {
        case 0: case 1: return 2;
        case 2: return 3;
        case 3: return 1;
        }
        }
        break;
    case 99:case 100:case 101:case 397:case 398:case 399:case 400:case 401:case 437:case 438:case 439:case 440:case 441:
        {
        /*组合模式为1,2时，单位为P量纲为0*/
        int i = m.prm_WorP->value().toInt();
        return (i==1||i==2) ? 0 : m.sys_dot_num->value().toInt() % 4;
        }
        break;
    case 104:case 141:case 167:
        {
        /*2组合模式为2时，单位为P量纲为0*/
        return m.prm_WorP->value().toInt() == 2 ? 0 : m.sys_dot_num->value().toInt() % 4;
        }
        break;
    case 133:case 210:case 211:case 212:case 213:case 214:case 215:case 216:case 217:case 218:case 219:case 270:case 271:
        return 1;
    }
    return 0;
}

QString MParam0::getUStr() const
{
    // auto code
    switch (sn) {
    case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:case 29:case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:case 41:case 42:case 43:case 44:case 45:case 46:case 47:case 48:case 49:case 50:case 51:case 52:case 53:case 54:case 55:case 56:case 57:case 58:case 59:case 60:case 63:case 64:case 65:case 66:case 67:case 68:case 69:case 71:case 72:case 73:case 74:case 75:case 77:case 78:case 83:case 84:case 85:case 86:case 87:case 88:case 89:case 90:case 93:case 94:case 95:case 96:case 97:case 98:case 103:case 106:case 107:case 108:case 109:case 110:case 111:case 112:case 113:case 114:case 115:case 116:case 117:case 118:case 119:case 120:case 121:case 122:case 123:case 124:case 125:case 126:case 127:case 132:case 133:case 134:case 135:case 137:case 142:case 144:case 147:case 148:case 158:case 164:case 168:case 169:case 170:case 171:case 172:case 174:case 175:case 176:case 177:case 178:case 179:case 180:case 181:case 182:case 183:case 184:case 205:case 206:case 207:case 208:case 209:case 272:case 283:case 334:case 335:case 336:case 337:case 338:case 339:case 340:case 341:case 342:case 343:case 344:case 345:case 346:case 347:case 348:case 349:case 350:case 351:case 352:case 353:case 354:case 355:case 356:case 357:case 358:case 359:case 360:case 361:case 362:case 363:case 364:case 365:case 366:case 367:case 368:case 369:case 370:case 371:case 372:case 373:case 374:case 375:case 376:case 377:case 378:case 379:case 380:case 381:case 382:case 383:case 384:case 385:case 386:case 387:case 388:case 389:case 390:case 391:case 392:case 393:case 394:case 395:case 396:case 402:case 403:case 404:case 405:case 406:case 412:case 413:case 414:case 415:case 416:case 417:case 418:case 419:case 420:case 421:case 422:case 423:case 424:case 425:case 426:case 432:case 433:case 434:case 435:case 436:case 442:case 443:case 444:case 445:case 446:case 447:case 528:case 529:case 530:case 531:case 532:case 533:case 534:case 535:case 536:case 537:case 538:case 539:case 540:case 541:case 542:case 543:case 544:case 545:case 546:case 547:case 548:case 549:case 550:case 551:case 552:case 553:case 554:case 555:case 556:case 557:case 558:case 559:case 560:case 561:case 562:case 563:case 564:case 565:case 566:case 567:case 568:case 569:case 570:case 571:case 572:case 573:case 574:case 575:case 576:case 577:case 578:case 579:case 580:case 581:case 582:case 583:case 584:case 585:case 586:case 587:case 588:case 589:case 590:case 591:case 592:case 593:case 594:case 595:case 596:case 597:case 598:case 599:case 600:case 601:case 602:case 603:case 604:case 605:case 606:case 607:case 608:case 609:case 610:case 611:case 612:case 613:case 614:case 615:case 616:case 617:case 618:case 619:case 620:case 621:case 622:case 623:case 624:case 625:case 626:case 627:case 628:case 629:case 630:case 631:case 632:case 633:case 634:case 635:case 636:case 639:case 640:case 641:case 642:case 643:case 644:case 645:case 646:case 647:case 648:case 649:case 650:case 653:case 654:case 655:
        return 0;
    case 61:case 62:case 76:case 79:case 81:case 128:case 129:case 136:case 138:case 140:case 163:case 185:case 186:case 187:case 188:case 189:case 190:case 191:case 192:case 193:case 194:case 195:case 196:case 197:case 198:case 199:case 200:case 201:case 202:case 203:case 204:case 427:case 428:case 429:case 430:case 431:case 448:case 449:case 450:case 451:case 452:case 453:case 454:case 455:case 456:case 457:case 458:case 459:case 460:case 461:case 462:case 463:case 464:case 465:case 466:case 467:case 468:case 469:case 470:case 471:case 472:case 473:case 474:case 475:case 476:case 477:case 478:case 479:case 480:case 481:case 482:case 483:case 484:case 485:case 486:case 487:case 488:case 489:case 490:case 491:case 492:case 493:case 494:case 495:case 496:case 497:case 498:case 499:case 500:case 501:case 502:case 503:case 504:case 505:case 506:case 507:case 508:case 509:case 510:case 511:case 512:case 513:case 514:case 515:case 516:case 517:case 518:case 519:case 520:case 521:case 522:case 523:case 524:case 525:case 526:case 527:case 637:case 638:case 651:case 652:
        {
        /*系统单位*/
        switch (m.sys_Unit->value().toInt()) {
        case 0: return "g";
        case 1: return "kg";
        case 2: return "lb";
        case 3: return "oz";
        }
        }
        break;
    case 70:case 145:
        return "min";
    case 80:case 82:case 165:case 166:
        return "kg";
    case 91:case 284:case 285:case 286:case 287:case 288:case 289:case 290:case 291:case 292:case 293:
        return "Hz";
    case 92:case 143:
        return "%";
    case 99:case 100:case 101:case 397:case 398:case 399:case 400:case 401:case 437:case 438:case 439:case 440:case 441:
        {
        /*组合模式为1,2时，单位为P量纲为0*/
        int i = m.prm_WorP->value().toInt();
        if (i==1||i==2)
            return "P";
        switch (m.sys_Unit->value().toInt()) {
        case 0: return "g";
        case 1: return "kg";
        case 2: return "lb";
        case 3: return "oz";
        }
        }
        break;
    case 102:case 407:case 408:case 409:case 410:case 411:
        return "ppm";
    case 104:case 141:case 167:
        {
        /*2组合模式为2时，单位为P量纲为0*/
        if (m.prm_WorP->value().toInt() == 2)
            return "P";
        switch (m.sys_Unit->value().toInt()) {
        case 0: return "g";
        case 1: return "kg";
        case 2: return "lb";
        case 3: return "oz";
        }
        }
        break;
    case 105:
        {
        /*主振模式为1时，范围为0~9999(ms)*/
        if (m.sys_DZJ_ZFSet->value().toInt() == 1)
            return "ms";
        }
        break;
    case 130:case 131:case 139:case 149:case 150:case 151:case 152:case 153:case 154:case 155:case 156:case 157:case 159:case 160:case 161:case 162:case 173:case 273:case 274:case 275:case 276:case 277:case 278:case 279:case 280:case 281:case 282:case 294:case 295:case 296:case 297:case 298:case 299:case 300:case 301:case 302:case 303:case 304:case 305:case 306:case 307:case 308:case 309:case 310:case 311:case 312:case 313:case 314:case 315:case 316:case 317:case 318:case 319:case 320:case 321:case 322:case 323:case 324:case 325:case 326:case 327:case 328:case 329:case 330:case 331:case 332:case 333:
        return "ms";
    case 146:
        return "s";
    case 210:case 211:case 212:case 213:case 214:case 215:case 216:case 217:case 218:case 219:case 270:case 271:
        return "°";
    case 220:case 221:case 222:case 223:case 224:case 225:case 226:case 227:case 228:case 229:case 230:case 231:case 232:case 233:case 234:case 235:case 236:case 237:case 238:case 239:case 240:case 241:case 242:case 243:case 244:case 245:case 246:case 247:case 248:case 249:
        return "rpm";
    case 250:case 251:case 252:case 253:case 254:case 255:case 256:case 257:case 258:case 259:case 260:case 261:case 262:case 263:case 264:case 265:case 266:case 267:case 268:case 269:
        return "rps²";
    }
    return 0;
}

QString MParam0::getRange() const
{
    // auto code
    switch (sn) {
    case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 28:case 29:case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:case 41:case 42:case 43:case 44:case 45:case 46:case 47:case 48:case 49:case 50:case 51:case 52:case 53:case 54:case 55:case 56:case 57:case 58:case 59:case 64:case 75:case 83:case 84:case 85:case 86:case 87:case 88:case 89:case 90:case 93:case 94:case 95:case 96:case 97:case 158:case 175:case 176:case 177:case 178:case 179:case 180:case 181:case 182:case 183:case 184:case 205:case 206:case 207:case 209:case 272:case 283:case 334:case 335:case 336:case 337:case 338:case 339:case 340:case 341:case 342:case 343:case 344:case 345:case 346:case 347:case 348:case 349:case 350:case 351:case 352:case 353:case 354:case 355:case 356:case 357:case 358:case 359:case 360:case 361:case 362:case 363:case 364:case 365:case 366:case 367:case 368:case 369:case 370:case 371:case 372:case 373:case 374:case 375:case 376:case 377:case 378:case 379:case 380:case 381:case 382:case 383:case 384:case 385:case 386:case 387:case 388:case 389:case 390:case 391:case 392:case 393:case 394:case 395:case 396:case 397:case 398:case 399:case 400:case 401:case 402:case 403:case 404:case 405:case 406:case 407:case 408:case 409:case 410:case 411:case 412:case 413:case 414:case 415:case 416:case 417:case 418:case 419:case 420:case 421:case 422:case 423:case 424:case 425:case 426:case 427:case 428:case 429:case 430:case 431:case 432:case 433:case 434:case 435:case 436:case 442:case 443:case 444:case 445:case 446:case 447:case 448:case 449:case 450:case 451:case 452:case 453:case 454:case 455:case 456:case 457:case 458:case 459:case 460:case 461:case 462:case 463:case 464:case 465:case 466:case 467:case 468:case 469:case 470:case 471:case 472:case 473:case 474:case 475:case 476:case 477:case 478:case 479:case 480:case 481:case 482:case 483:case 484:case 485:case 486:case 487:case 488:case 489:case 490:case 491:case 492:case 493:case 494:case 495:case 496:case 497:case 498:case 499:case 500:case 501:case 502:case 503:case 504:case 505:case 506:case 507:case 508:case 509:case 510:case 511:case 512:case 513:case 514:case 515:case 516:case 517:case 518:case 519:case 520:case 521:case 522:case 523:case 524:case 525:case 526:case 527:case 528:case 529:case 530:case 531:case 532:case 533:case 534:case 535:case 536:case 537:case 538:case 539:case 540:case 541:case 542:case 543:case 544:case 545:case 546:case 547:case 548:case 549:case 550:case 551:case 552:case 553:case 554:case 555:case 556:case 557:case 558:case 559:case 560:case 561:case 562:case 563:case 564:case 565:case 566:case 567:case 568:case 569:case 570:case 571:case 572:case 573:case 574:case 575:case 576:case 577:case 578:case 579:case 580:case 581:case 582:case 583:case 584:case 585:case 586:case 587:case 588:case 589:case 590:case 591:case 592:case 593:case 594:case 595:case 596:case 597:case 598:case 599:case 600:case 601:case 602:case 603:case 604:case 605:case 606:case 607:case 608:case 609:case 610:case 611:case 612:case 613:case 614:case 615:case 616:case 617:case 618:case 619:case 620:case 621:case 622:case 623:case 624:case 625:case 626:case 627:case 628:case 629:case 630:case 631:case 632:case 633:case 634:case 635:case 636:case 637:case 638:case 639:case 640:case 641:case 642:case 643:case 644:case 645:case 646:case 647:case 648:case 649:case 650:case 651:case 652:case 653:case 654:case 655:
        return 0;
    case 18:case 19:case 134:case 137:case 147:
        return "1 ~ 99";
    case 20:
        return "5 ~ 40";
    case 21:case 22:case 63:case 65:case 67:case 74:case 77:
        return "0 ~ 1";
    case 23:
        return "0, 1";
    case 24:case 25:case 72:case 103:case 132:
        return "0 ~ 3";
    case 26:
        return m.sys_JLD_state->value().toInt() == 0 ? "0" : "0 ~ 2";
    case 27:case 174:
        return "0 ~ 15";
    case 60:
        return QString("%1 ~ %2").arg(vPS(1),vPSU(1000));
    case 61:case 62:case 79:case 80:case 81:case 82:case 136:case 138:
        return QString("%1 ~ %2").arg(vPS(1),vPSU(32000));
    case 66:
        return "0:g, 1:kg, 2:lb, 3:oz";
    case 68:case 71:
        return "0 ~ 2";
    case 69:case 91:
        return "40 ~ 70 Hz";
    case 70:
        return "0 ~ 99 min";
    case 73:
        return "1 ~ 9";
    case 76:
        return QString("%1 ~ %2").arg(vPS(50), vPSU(9999));
    case 78:
        return QString("%1 ~ %2").arg(vPS(500), vPSU(1500));
    case 92:
        return "50 ~ 150 %";
    case 98:
        return "S```0`16";
    case 99:
        return QString("%1 ~ %2").arg(vPS(1),vPSU(99999));
    case 100:
        return QString("%1 ~ %2").arg(vPS(0),vPSU(99999));
    case 101:
        return QString("%1 ~ %2").arg(vPS(0),m.prm_SetWeight->vStrU());
    case 102:
        {
        /*包装速度*/
        if (m.sys_RunMode->value().toInt() == 0)
            return "5 ~ 95 ppm";
        else
            return "5 ~ 160 ppm";
        }
        break;
    case 104:
        {
        /*2组合模式为2时，范围为-99~99(P),否则-999~999(P)*/
        if (m.prm_WorP->value().toInt() == 2)
            return "-99 ~ 99 P";
        else
            return QString("%1 ~ %2").arg(vPS(-999),vPSU(999));
        }
        break;
    case 105:
        {
        /*主振模式为1时，范围为0~9999(ms),否则1~999*/
        if (m.sys_DZJ_ZFSet->value().toInt() == 1)
            return "0 ~ 9999 ms";
        else
            return "1 ~ 999";
        }
        break;
    case 106:case 107:case 108:case 109:case 110:case 111:case 112:case 113:case 114:case 115:case 116:case 117:case 118:case 119:case 120:case 121:case 122:case 123:case 124:case 125:case 126:case 127:
        return "1 ~ 999";
    case 128:case 129:
        return QString("%1 ~ %2").arg(vPS(1),vPSU(999));
    case 130:
        {
        /*主振模式为1时，范围为0~9999(ms),否则10~9999(ms)*/
        if (m.sys_DZJ_ZFSet->value().toInt() == 1)
            return "0 ~ 9999 ms";
        else
            return "10 ~ 9999 ms";
        }
        break;
    case 131:
        return "10 ~ 5000 ms";
    case 133:
        return "1.0 ~ 7.0";
    case 135:
        return "0.1 ~ 99.9";
    case 139:
        {
        /* 1~线振时间-1 */
        return QString("%1 ~ %2").arg(vPS(1),vPSU(m.prm_DZJ_X_RunTime->valuenp().toInt() - 1));
        }
        break;
    case 140:
        return QString("%1 ~ %2").arg(vPS(1),vPSU(9999));
    case 141:
        {
        /* 0~目标值-1,组合模式1时0~目标值*DWW-1 */
        int dww = 1;
        if (m.prm_WorP->value().toInt() == 1)
            dww = m.prm_DWW->valuenp().toInt();
        return QString("%1 ~ %2").arg(vPS(0),vPSU(m.prm_SetWeight->valuenp().toInt()*dww - 1));
        }
        break;
    case 142:case 208:
        return "0 ~ 999";
    case 143:
        return "1 ~ 99 %";
    case 144:
        return "1 ~ 9999";
    case 145:
        return "0 ~ 999 min";
    case 146:
        return "1 ~ 999 s";
    case 148:
        {
        /* 1~多次放料次数 */
        return QString("%1 ~ %2").arg(vPS(1),m.prm_MFL_Cnt->vStrU());
        }
        break;
    case 149:case 151:case 155:
        {
        /* 1~Min(60000/speed,9999) */
        int max = 9999;
        int speed = m.prm_speed->value().toInt();
        if (speed > 0)
            max = qMin(60000/speed,9999);
        return QString("%1 ~ %2").arg(vPS(1),vPSU(max));
        }
        break;
    case 150:case 152:case 154:case 273:case 274:case 275:case 276:case 277:case 278:case 279:case 280:case 281:case 282:
        return "1 ~ 9999 ms";
    case 153:
        {
        /* -集料斗延时~9999 */
        return QString("-%1 ~ %2").arg(m.prm_JLD_Dly->vStr(),vPSU(9999));
        }
        break;
    case 156:
        return "400 ~ 3000 ms";
    case 157:
        return "0 ~ 9999 ms";
    case 159:case 160:case 161:case 162:
        return "0 ~ 2000 ms";
    case 163:
        return QString("%1 ~ %2").arg(vPS(0),vPSU(m.prm_DWW->vIntNp()-1));
    case 164:
        return "0 ~ 99 %";
    case 165:case 166:
        return QString("%1 ~ %2").arg(vPS(1),vPSU(3000));
    case 167:
        {
        /* 0~目标值/2-1,组合模式1时0~目标值*DWW/2-1 */
        int dww = 1;
        if (m.prm_WorP->value().toInt() == 1)
            dww = m.prm_DWW->valuenp().toInt();
        return QString("%1 ~ %2").arg(vPS(0),vPSU(m.prm_SetWeight->valuenp().toInt()*dww/2 - 1));
        }
        break;
    case 168:case 169:case 170:case 171:case 172:
        return "0 ~ 4";
    case 173:
        {
        /* -记忆斗延时~9999 */
        return QString("-%1 ~ %2").arg(m.prm_JYD_Dly->vStr(),vPSU(9999));
        }
        break;
    case 185:case 186:case 187:case 188:case 189:case 190:case 191:case 192:case 193:case 194:case 195:case 196:case 197:case 198:case 199:case 200:case 201:case 202:case 203:case 204:
        return QString("%1 ~ %2").arg(vPS(0),vPSU(10000));
    case 210:case 211:case 212:case 213:case 214:case 215:case 216:case 217:case 218:case 219:
        return "-360.0 ~ -0.1 , 0.1 ~ 360.0 °";
    case 220:case 221:case 222:case 223:case 224:case 225:case 226:case 227:case 228:case 229:
        return "0, 10 ~ 600 rpm";
    case 230:case 231:case 232:case 233:case 234:case 235:case 236:case 237:case 238:case 239:case 240:case 241:case 242:case 243:case 244:case 245:case 246:case 247:case 248:case 249:
        return "5 ~ 600 rpm";
    case 250:case 251:case 252:case 253:case 254:case 255:case 256:case 257:case 258:case 259:case 260:case 261:case 262:case 263:case 264:case 265:case 266:case 267:case 268:case 269:
        return "50 ~ 9999 rps²";
    case 270:
        return "0 ~ 360.0 °";
    case 271:
        return "5.0 ~ 360.0 °";
    case 284:case 285:case 286:case 287:case 288:case 289:case 290:case 291:case 292:case 293:
        return "-9999 ~ -100 , 100 ~ 9999 Hz";
    case 294:case 295:case 296:case 297:case 298:case 299:case 300:case 301:case 302:case 303:case 314:case 315:case 316:case 317:case 318:case 319:case 320:case 321:case 322:case 323:
        return "0, 50 ~ 9999 ms";
    case 304:case 305:case 306:case 307:case 308:case 309:case 310:case 311:case 312:case 313:case 324:case 325:case 326:case 327:case 328:case 329:case 330:case 331:case 332:case 333:
        return "0 ~ 60000 ms";
    case 437:case 438:case 439:case 440:case 441:
        break;
    }
    return 0;
}

QString MParam0::vStr(const QVariant &value) const
{
    // auto code
    switch (sn) {
    case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:case 29:case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:case 41:case 42:case 43:case 44:case 45:case 46:case 47:case 48:case 49:case 50:case 51:case 52:case 53:case 54:case 55:case 56:case 57:case 58:case 59:case 60:case 61:case 62:case 63:case 64:case 65:case 66:case 67:case 68:case 69:case 70:case 71:case 72:case 73:case 74:case 75:case 76:case 77:case 78:case 79:case 80:case 81:case 82:case 83:case 84:case 85:case 86:case 87:case 88:case 89:case 90:case 91:case 92:case 93:case 94:case 95:case 96:case 97:case 98:case 99:case 100:case 101:case 102:case 103:case 104:case 105:case 106:case 107:case 108:case 109:case 110:case 111:case 112:case 113:case 114:case 115:case 116:case 117:case 118:case 119:case 120:case 121:case 122:case 123:case 124:case 125:case 126:case 127:case 128:case 129:case 130:case 131:case 132:case 133:case 134:case 135:case 136:case 137:case 138:case 139:case 140:case 141:case 142:case 143:case 144:case 145:case 146:case 147:case 148:case 149:case 150:case 151:case 152:case 153:case 154:case 155:case 156:case 157:case 158:case 159:case 160:case 161:case 162:case 163:case 164:case 165:case 166:case 167:case 168:case 169:case 170:case 171:case 172:case 173:case 174:case 175:case 176:case 177:case 178:case 179:case 180:case 181:case 182:case 183:case 184:case 185:case 186:case 187:case 188:case 189:case 190:case 191:case 192:case 193:case 194:case 195:case 196:case 197:case 198:case 199:case 200:case 201:case 202:case 203:case 204:case 205:case 206:case 207:case 208:case 209:case 210:case 211:case 212:case 213:case 214:case 215:case 216:case 217:case 218:case 219:case 220:case 221:case 222:case 223:case 224:case 225:case 226:case 227:case 228:case 229:case 230:case 231:case 232:case 233:case 234:case 235:case 236:case 237:case 238:case 239:case 240:case 241:case 242:case 243:case 244:case 245:case 246:case 247:case 248:case 249:case 250:case 251:case 252:case 253:case 254:case 255:case 256:case 257:case 258:case 259:case 260:case 261:case 262:case 263:case 264:case 265:case 266:case 267:case 268:case 269:case 270:case 271:case 272:case 273:case 274:case 275:case 276:case 277:case 278:case 279:case 280:case 281:case 282:case 283:case 284:case 285:case 286:case 287:case 288:case 289:case 290:case 291:case 292:case 293:case 294:case 295:case 296:case 297:case 298:case 299:case 300:case 301:case 302:case 303:case 304:case 305:case 306:case 307:case 308:case 309:case 310:case 311:case 312:case 313:case 314:case 315:case 316:case 317:case 318:case 319:case 320:case 321:case 322:case 323:case 324:case 325:case 326:case 327:case 328:case 329:case 330:case 331:case 332:case 333:case 334:case 335:case 336:case 337:case 338:case 339:case 340:case 341:case 342:case 343:case 344:case 345:case 346:case 347:case 348:case 349:case 350:case 351:case 352:case 353:case 354:case 355:case 356:case 357:case 358:case 359:case 360:case 361:case 362:case 363:case 364:case 365:case 366:case 367:case 368:case 369:case 370:case 371:case 372:case 373:case 374:case 375:case 376:case 377:case 378:case 379:case 380:case 381:case 382:case 383:case 384:case 385:case 386:case 387:case 388:case 389:case 390:case 391:case 392:case 393:case 394:case 395:case 396:case 397:case 398:case 399:case 400:case 401:case 402:case 403:case 404:case 405:case 406:case 407:case 408:case 409:case 410:case 411:case 412:case 413:case 414:case 415:case 416:case 417:case 418:case 419:case 420:case 421:case 422:case 423:case 424:case 425:case 426:case 427:case 428:case 429:case 430:case 431:case 432:case 433:case 434:case 435:case 436:case 437:case 438:case 439:case 440:case 441:case 442:case 443:case 444:case 445:case 446:case 447:case 448:case 449:case 450:case 451:case 452:case 453:case 454:case 455:case 456:case 457:case 458:case 459:case 460:case 461:case 462:case 463:case 464:case 465:case 466:case 467:case 468:case 469:case 470:case 471:case 472:case 473:case 474:case 475:case 476:case 477:case 478:case 479:case 480:case 481:case 482:case 483:case 484:case 485:case 486:case 487:case 488:case 489:case 490:case 491:case 492:case 493:case 494:case 495:case 496:case 497:case 498:case 499:case 500:case 501:case 502:case 503:case 504:case 505:case 506:case 507:case 508:case 509:case 510:case 511:case 512:case 513:case 514:case 515:case 516:case 517:case 518:case 519:case 520:case 521:case 522:case 523:case 524:case 525:case 526:case 527:case 528:case 529:case 530:case 531:case 532:case 533:case 534:case 535:case 536:case 537:case 538:case 539:case 540:case 541:case 542:case 543:case 544:case 545:case 546:case 547:case 548:case 549:case 550:case 551:case 552:case 553:case 554:case 555:case 556:case 557:case 558:case 559:case 560:case 561:case 562:case 563:case 564:case 565:case 566:case 567:case 568:case 569:case 570:case 571:case 572:case 573:case 574:case 575:case 576:case 577:case 578:case 579:case 580:case 581:case 582:case 583:case 584:case 585:case 586:case 587:case 588:case 589:case 590:case 591:case 592:case 593:case 594:case 595:case 596:case 597:case 598:case 599:case 600:case 601:case 602:case 603:case 604:case 605:case 606:case 607:case 608:case 609:case 610:case 611:case 612:case 613:case 614:case 615:case 616:case 617:case 618:case 619:case 620:case 621:case 622:case 623:case 624:case 625:case 626:case 627:case 628:case 629:case 630:case 631:case 632:case 633:case 634:case 635:case 636:case 637:case 638:case 639:case 640:case 641:case 642:case 643:case 644:case 645:case 646:case 647:case 648:case 649:case 650:case 651:case 652:case 653:case 654:case 655:
        break;
    }
    return MParam::vStr(value);
}

QVariant MParam0::sVal(const QString &str) const
{
    // auto code
    switch (sn) {
    case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:case 29:case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:case 41:case 42:case 43:case 44:case 45:case 46:case 47:case 48:case 49:case 50:case 51:case 52:case 53:case 54:case 55:case 56:case 57:case 58:case 59:case 60:case 61:case 62:case 63:case 64:case 65:case 66:case 67:case 68:case 69:case 70:case 71:case 72:case 73:case 74:case 75:case 76:case 77:case 78:case 79:case 80:case 81:case 82:case 83:case 84:case 85:case 86:case 87:case 88:case 89:case 90:case 91:case 92:case 93:case 94:case 95:case 96:case 97:case 98:case 99:case 100:case 101:case 102:case 103:case 104:case 105:case 106:case 107:case 108:case 109:case 110:case 111:case 112:case 113:case 114:case 115:case 116:case 117:case 118:case 119:case 120:case 121:case 122:case 123:case 124:case 125:case 126:case 127:case 128:case 129:case 130:case 131:case 132:case 133:case 134:case 135:case 136:case 137:case 138:case 139:case 140:case 141:case 142:case 143:case 144:case 145:case 146:case 147:case 148:case 149:case 150:case 151:case 152:case 153:case 154:case 155:case 156:case 157:case 158:case 159:case 160:case 161:case 162:case 163:case 164:case 165:case 166:case 167:case 168:case 169:case 170:case 171:case 172:case 173:case 174:case 175:case 176:case 177:case 178:case 179:case 180:case 181:case 182:case 183:case 184:case 185:case 186:case 187:case 188:case 189:case 190:case 191:case 192:case 193:case 194:case 195:case 196:case 197:case 198:case 199:case 200:case 201:case 202:case 203:case 204:case 205:case 206:case 207:case 208:case 209:case 210:case 211:case 212:case 213:case 214:case 215:case 216:case 217:case 218:case 219:case 220:case 221:case 222:case 223:case 224:case 225:case 226:case 227:case 228:case 229:case 230:case 231:case 232:case 233:case 234:case 235:case 236:case 237:case 238:case 239:case 240:case 241:case 242:case 243:case 244:case 245:case 246:case 247:case 248:case 249:case 250:case 251:case 252:case 253:case 254:case 255:case 256:case 257:case 258:case 259:case 260:case 261:case 262:case 263:case 264:case 265:case 266:case 267:case 268:case 269:case 270:case 271:case 272:case 273:case 274:case 275:case 276:case 277:case 278:case 279:case 280:case 281:case 282:case 283:case 284:case 285:case 286:case 287:case 288:case 289:case 290:case 291:case 292:case 293:case 294:case 295:case 296:case 297:case 298:case 299:case 300:case 301:case 302:case 303:case 304:case 305:case 306:case 307:case 308:case 309:case 310:case 311:case 312:case 313:case 314:case 315:case 316:case 317:case 318:case 319:case 320:case 321:case 322:case 323:case 324:case 325:case 326:case 327:case 328:case 329:case 330:case 331:case 332:case 333:case 334:case 335:case 336:case 337:case 338:case 339:case 340:case 341:case 342:case 343:case 344:case 345:case 346:case 347:case 348:case 349:case 350:case 351:case 352:case 353:case 354:case 355:case 356:case 357:case 358:case 359:case 360:case 361:case 362:case 363:case 364:case 365:case 366:case 367:case 368:case 369:case 370:case 371:case 372:case 373:case 374:case 375:case 376:case 377:case 378:case 379:case 380:case 381:case 382:case 383:case 384:case 385:case 386:case 387:case 388:case 389:case 390:case 391:case 392:case 393:case 394:case 395:case 396:case 397:case 398:case 399:case 400:case 401:case 402:case 403:case 404:case 405:case 406:case 407:case 408:case 409:case 410:case 411:case 412:case 413:case 414:case 415:case 416:case 417:case 418:case 419:case 420:case 421:case 422:case 423:case 424:case 425:case 426:case 427:case 428:case 429:case 430:case 431:case 432:case 433:case 434:case 435:case 436:case 437:case 438:case 439:case 440:case 441:case 442:case 443:case 444:case 445:case 446:case 447:case 448:case 449:case 450:case 451:case 452:case 453:case 454:case 455:case 456:case 457:case 458:case 459:case 460:case 461:case 462:case 463:case 464:case 465:case 466:case 467:case 468:case 469:case 470:case 471:case 472:case 473:case 474:case 475:case 476:case 477:case 478:case 479:case 480:case 481:case 482:case 483:case 484:case 485:case 486:case 487:case 488:case 489:case 490:case 491:case 492:case 493:case 494:case 495:case 496:case 497:case 498:case 499:case 500:case 501:case 502:case 503:case 504:case 505:case 506:case 507:case 508:case 509:case 510:case 511:case 512:case 513:case 514:case 515:case 516:case 517:case 518:case 519:case 520:case 521:case 522:case 523:case 524:case 525:case 526:case 527:case 528:case 529:case 530:case 531:case 532:case 533:case 534:case 535:case 536:case 537:case 538:case 539:case 540:case 541:case 542:case 543:case 544:case 545:case 546:case 547:case 548:case 549:case 550:case 551:case 552:case 553:case 554:case 555:case 556:case 557:case 558:case 559:case 560:case 561:case 562:case 563:case 564:case 565:case 566:case 567:case 568:case 569:case 570:case 571:case 572:case 573:case 574:case 575:case 576:case 577:case 578:case 579:case 580:case 581:case 582:case 583:case 584:case 585:case 586:case 587:case 588:case 589:case 590:case 591:case 592:case 593:case 594:case 595:case 596:case 597:case 598:case 599:case 600:case 601:case 602:case 603:case 604:case 605:case 606:case 607:case 608:case 609:case 610:case 611:case 612:case 613:case 614:case 615:case 616:case 617:case 618:case 619:case 620:case 621:case 622:case 623:case 624:case 625:case 626:case 627:case 628:case 629:case 630:case 631:case 632:case 633:case 634:case 635:case 636:case 637:case 638:case 639:case 640:case 641:case 642:case 643:case 644:case 645:case 646:case 647:case 648:case 649:case 650:case 651:case 652:case 653:case 654:case 655:
        break;
    }
    return MParam::sVal(str);
}

MachineParamList0::MachineParamList0() :
    MachineParamList(2006)
{
    // auto code init mp
    for (int i = 0; i < 656; i++) mps[i] = new MParam0(this, i);
    m.sys_union = mps[0];
    m.prm_union = mps[1];
    m.motor_union = mps[2];
    m.prmlist_union = mps[3];
    m.cmd_param = mps[4];
    m.cmd = mps[5];
    m.runstatus_union = mps[6];
    m.doustatus_union = mps[7];
    m.databuf_union = mps[8];
    m.sys_CPUID = mps[9];
    m.sys_MacType = mps[10];
    m.sys_MacVer = mps[11];
    m.sys_SysTime = mps[12];
    for (int i=0; i<5; i++) m.sys_prm_ids[i] = mps[13+i];
    m.sys_SXBS = mps[18];
    m.sys_YH_Combin = mps[19];
    m.sys_QZPF_DS = mps[20];
    m.sys_JLD_state = mps[21];
    m.sys_LWJC_FS = mps[22];
    m.sys_QP_2CJL = mps[23];
    m.sys_dot_num = mps[24];
    m.sys_TrackMode = mps[25];
    m.sys_FL_FS = mps[26];
    m.sys_ASF = mps[27];
    for (int i=0; i<32; i++) m.sys_DOU_State[i] = mps[28+i];
    m.sys_FDZ = mps[60];
    m.sys_nov_zd = mps[61];
    m.sys_max_zd = mps[62];
    m.sys_PLCJMS = mps[63];
    m._unused = mps[64];
    m.sys_YC_FLFS = mps[65];
    m.sys_Unit = mps[66];
    m.sys_ZDSense = mps[67];
    m.sys_DZJ_ZFSet = mps[68];
    m.sys_DZJ_PLSet = mps[69];
    m.sys_QXResetTime = mps[70];
    m.sys_RunMode = mps[71];
    m.sys_AutoZeroMode = mps[72];
    m.sys_div = mps[73];
    m.sys_QPDGSel = mps[74];
    m.sys_JLXHmode = mps[75];
    m.sys_AutoZeroWeight = mps[76];
    m.sys_VibrationSen = mps[77];
    m.sys_VibrationCoe = mps[78];
    m.sys_nov_czd = mps[79];
    m.sys_nov_zzj = mps[80];
    m.sys_max_czd = mps[81];
    m.sys_max_zzj = mps[82];
    m.sys_BurdeningOpt = mps[83];
    for (int i=0; i<5; i++) m.sys_RecordCounts[i] = mps[84+i];
    m.sys_ioin_pin = mps[89];
    m.sys_ioout_pin = mps[90];
    m.sys_DZJ_FreqSet = mps[91];
    m.sys_DZJ_GainSet = mps[92];
    m.sys_ioin_status = mps[93];
    m.sys_ioout_status = mps[94];
    m.sys_bQXFlag = mps[95];
    m._sys_end = mps[96];
    m.prm_id = mps[97];
    m.prm_name = mps[98];
    m.prm_SetWeight = mps[99];
    m.prm_SetWeight1 = mps[100];
    m.prm_SetWeight2 = mps[101];
    m.prm_speed = mps[102];
    m.prm_WorP = mps[103];
    m.prm_PCL = mps[104];
    m.prm_DZJ_Z_ZF = mps[105];
    for (int i=0; i<22; i++) m.prm_DZJ_X_ZF[i] = mps[106+i];
    m.prm_DWW_UpLimit = mps[128];
    m.prm_DWW_DownLimit = mps[129];
    m.prm_DZJ_Z_RunTime = mps[130];
    m.prm_DZJ_X_RunTime = mps[131];
    m.prm_AFCMode = mps[132];
    m.prm_AFCT_TDS = mps[133];
    m.prm_AFCT_Cnt = mps[134];
    m.prm_AFCT_LDS = mps[135];
    m.prm_AFCI_DDW = mps[136];
    m.prm_AFCI_Cnt = mps[137];
    m.prm_AFCI_LW = mps[138];
    m.prm_AFCR_Time = mps[139];
    m.prm_DWW = mps[140];
    m.prm_OverWeight = mps[141];
    m.prm_P2Cnt = mps[142];
    m.prm_WL_check = mps[143];
    m.prm_NoSel_check = mps[144];
    m.prm_AutoZeroTime = mps[145];
    m.prm_LDJL_Time = mps[146];
    m.prm_MFL_Cnt = mps[147];
    m.prm_JLD_MFLNum = mps[148];
    m.prm_HCD_Dly = mps[149];
    m.prm_CZD_Dly = mps[150];
    m.prm_JYD_Dly = mps[151];
    m.prm_JLD_Dly = mps[152];
    m.prm_JLD_OpenTime = mps[153];
    m.prm_FL_dly = mps[154];
    m.prm_FL_time = mps[155];
    m.prm_MSV_Dly = mps[156];
    m.prm_ZHFL_time = mps[157];
    m.prm_res = mps[158];
    for (int i=0; i<4; i++) m.prm_Dou_PauseTime[i] = mps[159+i];
    m.prm_TWW = mps[163];
    m.prm_ZZJ_QLTJ = mps[164];
    m.prm_ZZJ_ULW = mps[165];
    m.prm_ZZJ_DLW = mps[166];
    m.prm_CombinWeight = mps[167];
    for (int i=0; i<4; i++) m.prm_MaDaMode[i] = mps[168+i];
    m.prm_ZZMDMode = mps[172];
    m.prm_JYD_Dly2 = mps[173];
    m.prm_ASF = mps[174];
    for (int i=0; i<10; i++) m.prm_ZDD_Dou[i] = mps[175+i];
    for (int i=0; i<10; i++) m.prm_ZDD_WeightUp[i] = mps[185+i];
    for (int i=0; i<10; i++) m.prm_ZDD_WeightDn[i] = mps[195+i];
    m.prm_ZDD_MultSel = mps[205];
    m.prm_ZDD_LowerQPSel = mps[206];
    m._prm_end = mps[207];
    m.prm_AutoZeroCnt = mps[208];
    m.motor_id = mps[209];
    for (int i=0; i<10; i++) m.motor_pos[i] = mps[210+i];
    for (int i=0; i<10; i++) m.motor_speed[i] = mps[220+i];
    for (int i=0; i<10; i++) m.motor_start[i] = mps[230+i];
    for (int i=0; i<10; i++) m.motor_end[i] = mps[240+i];
    for (int i=0; i<10; i++) m.motor_acc[i] = mps[250+i];
    for (int i=0; i<10; i++) m.motor_dec[i] = mps[260+i];
    m.motor_gdtzjd = mps[270];
    m.motor_wgdtzjd = mps[271];
    m._motor_end = mps[272];
    for (int i=0; i<10; i++) m.motor_stoptime[i] = mps[273+i];
    m.motor2_id = mps[283];
    for (int i=0; i<10; i++) m.motor2_sp[i] = mps[284+i];
    for (int i=0; i<10; i++) m.motor2_ta[i] = mps[294+i];
    for (int i=0; i<10; i++) m.motor2_tm[i] = mps[304+i];
    for (int i=0; i<10; i++) m.motor2_tb[i] = mps[314+i];
    for (int i=0; i<10; i++) m.motor2_ts[i] = mps[324+i];
    m._motor2_end = mps[334];
    m.prmlist_id = mps[335];
    for (int i=0; i<10; i++) m.prmlist_name[i] = mps[336+i];
    for (int i=0; i<10; i++) m.prmlist_SetWeight[i] = mps[346+i];
    for (int i=0; i<10; i++) m.prmlist_SetWeight1[i] = mps[356+i];
    for (int i=0; i<10; i++) m.prmlist_SetWeight2[i] = mps[366+i];
    for (int i=0; i<10; i++) m.prmlist_speed[i] = mps[376+i];
    for (int i=0; i<10; i++) m.prmlist_WorP[i] = mps[386+i];
    m._prmlist_end = mps[396];
    for (int i=0; i<5; i++) m.cmb_Weight[i] = mps[397+i];
    for (int i=0; i<5; i++) m.cmb_cbtype[i] = mps[402+i];
    for (int i=0; i<5; i++) m.cmb_speed[i] = mps[407+i];
    for (int i=0; i<5; i++) m.cmb_RunStatus[i] = mps[412+i];
    for (int i=0; i<5; i++) m.cmb_AlarmDOu[i] = mps[417+i];
    for (int i=0; i<5; i++) m.cmb_combinCount[i] = mps[422+i];
    for (int i=0; i<5; i++) m.cmb_DWW[i] = mps[427+i];
    for (int i=0; i<5; i++) m.cmb_DouStatus[i] = mps[432+i];
    for (int i=0; i<5; i++) m.cmb_Weight2[i] = mps[437+i];
    for (int i=0; i<5; i++) m.cmb_combinCount2[i] = mps[442+i];
    m._cmb_end = mps[447];
    for (int i=0; i<80; i++) m.doustatus[i] = mps[448+i];
    m.mod_totaln = mps[528];
    for (int i=0; i<35; i++) m.mod_ver[i] = mps[529+i];
    for (int i=0; i<35; i++) m.mod_status0[i] = mps[564+i];
    for (int i=0; i<35; i++) m.mod_status1[i] = mps[599+i];
    m._mod_end = mps[634];
    m.samp_dounum = mps[635];
    m.samp_time = mps[636];
    m.samp_max = mps[637];
    m.samp_weight = mps[638];
    m.samp_datasize = mps[639];
    for (int i=0; i<10; i++) m.samp_data[i] = mps[640+i];
    m._samp_end = mps[650];
    m.zdtest_dou_weight = mps[651];
    m.zdtest_zd_weight = mps[652];
    m.zdtest_cpt = mps[653];
    m._zdtest_end = mps[654];
    m._end = mps[655];

    // auto code init mmp
    
    MParam *_First[][2] = {{m.sys_CPUID,m._sys_end},{m.prm_id,m._prm_end}}; initMMParam(mm.First, _First, 2);
    MParam *_Sys[][2] = {{m.sys_CPUID,m._sys_end}}; initMMParam(mm.Sys, _Sys, 1);
    MParam *_Sys0[][2] = {{m.sys_CPUID,m.sys_VibrationCoe}}; initMMParam(mm.Sys0, _Sys0, 1);
    MParam *_Calib[][2] = {{m.sys_nov_czd,m.sys_max_zzj}}; initMMParam(mm.Calib, _Calib, 1);
    MParam *_IO_Set[][2] = {{m.sys_ioin_pin,m.sys_ioout_pin}}; initMMParam(mm.IO_Set, _IO_Set, 1);
    MParam *_IO_Status[][2] = {{m.sys_ioin_status,m.sys_ioout_status}}; initMMParam(mm.IO_Status, _IO_Status, 1);
    MParam *_PLZY[][2] = {{m.sys_DZJ_FreqSet,m.sys_DZJ_GainSet}}; initMMParam(mm.PLZY, _PLZY, 1);
    MParam *_Prm[][2] = {{m.prm_id,m._prm_end}}; initMMParam(mm.Prm, _Prm, 1);
    MParam *_Motor[][2] = {{m.motor_id,m._motor_end}}; initMMParam(mm.Motor, _Motor, 1);
    MParam *_Motor2[][2] = {{m.motor2_id,m._motor2_end}}; initMMParam(mm.Motor2, _Motor2, 1);
    MParam *_PrmList[][2] = {{m.prmlist_id,m._prmlist_end}}; initMMParam(mm.PrmList, _PrmList, 1);
    MParam *_Mod[][2] = {{m.mod_totaln,m._mod_end}}; initMMParam(mm.Mod, _Mod, 1);
    MParam *_Samp[][2] = {{m.samp_dounum,m._samp_end}}; initMMParam(mm.Samp, _Samp, 1);
    MParam *_ZdTest[][2] = {{m.zdtest_dou_weight,m._zdtest_end}}; initMMParam(mm.ZdTest, _ZdTest, 1);
    mm.sys_prm_ids.append(MRegRWInfo(15,5,200,200));
    mm.sys_DOU_State.append(MRegRWInfo(30,32,200,200));
    mm.sys_RecordCounts.append(MRegRWInfo(101,5,200,200));
    mm.prm_DZJ_X_ZF.append(MRegRWInfo(277,22,200,200));
    mm.prm_Dou_PauseTime.append(MRegRWInfo(330,4,200,200));
    mm.prm_MaDaMode.append(MRegRWInfo(342,4,200,200));
    mm.prm_ZDD_Dou.append(MRegRWInfo(349,10,200,200));
    mm.prm_ZDD_WeightUp.append(MRegRWInfo(359,10,200,200));
    mm.prm_ZDD_WeightDn.append(MRegRWInfo(369,10,200,200));
    mm.motor_pos.append(MRegRWInfo(514,10,200,200));
    mm.motor_speed.append(MRegRWInfo(515,10,200,200));
    mm.motor_start.append(MRegRWInfo(516,10,200,200));
    mm.motor_end.append(MRegRWInfo(517,10,200,200));
    mm.motor_acc.append(MRegRWInfo(518,10,200,200));
    mm.motor_dec.append(MRegRWInfo(519,10,200,200));
    mm.motor_stoptime.append(MRegRWInfo(514,10,200,200));
    mm.motor2_sp.append(MRegRWInfo(514,10,200,200));
    mm.motor2_ta.append(MRegRWInfo(515,10,200,200));
    mm.motor2_tm.append(MRegRWInfo(516,10,200,200));
    mm.motor2_tb.append(MRegRWInfo(517,10,200,200));
    mm.motor2_ts.append(MRegRWInfo(518,10,200,200));
    mm.prmlist_name.append(MRegRWInfo(635,100,200,200));
    mm.prmlist_SetWeight.append(MRegRWInfo(645,20,200,200));
    mm.prmlist_SetWeight1.append(MRegRWInfo(647,20,200,200));
    mm.prmlist_SetWeight2.append(MRegRWInfo(649,20,200,200));
    mm.prmlist_speed.append(MRegRWInfo(651,10,200,200));
    mm.prmlist_WorP.append(MRegRWInfo(652,10,200,200));
    mm.cmb_Weight.append(MRegRWInfo(817,10,200,200));
    mm.cmb_cbtype.append(MRegRWInfo(819,5,200,200));
    mm.cmb_speed.append(MRegRWInfo(820,5,200,200));
    mm.cmb_RunStatus.append(MRegRWInfo(821,5,200,200));
    mm.cmb_AlarmDOu.append(MRegRWInfo(822,10,200,200));
    mm.cmb_combinCount.append(MRegRWInfo(824,5,200,200));
    mm.cmb_DWW.append(MRegRWInfo(825,5,200,200));
    mm.cmb_DouStatus.append(MRegRWInfo(826,25,200,200));
    mm.cmb_Weight2.append(MRegRWInfo(831,10,200,200));
    mm.cmb_combinCount2.append(MRegRWInfo(833,5,200,200));
    mm.doustatus.append(MRegRWInfo(902,80,200,200));
    mm.mod_ver.append(MRegRWInfo(983,140,200,200));
    mm.mod_status0.append(MRegRWInfo(987,35,200,200));
    mm.mod_status1.append(MRegRWInfo(988,35,200,200));
    mm.samp_data.append(MRegRWInfo(987,1000,200,200));

    // auto code init firstmmp
    firstReadMpslist.append(mm.First);
}

MachineParamList0::~MachineParamList0()
{
    // auto code delete mp
    for (int i = 0; i < 656; i++) delete mps[i];
}

void MachineParamList0::initMMParam(QList<const MParam *> &mmp, MParam *mmp2[][2], int len)
{
    for (int i = 0; i < len; i++) {
        for (int j = ((MParam0 *)mmp2[i][0])->sn; j <= ((MParam0 *)mmp2[i][1])->sn; j++)
            mmp.append(mps[j]);
    }
}

void MachineParamList0::initMMParam(QList<MRegRWInfo> &mmp, MParam *mmp2[][2], int len)
{
    QList<MRegRWInfo> list;
    for (int i = 0; i < len; i++) {
        int j = ((MParam0 *)mmp2[i][0])->sn;
        MRegRWInfo m;
        m.addr = mps[j]->getAddr();
        m.count = mps[j]->getCount();
        m.rdelay = mps[j]->getRDelay();
        m.wdelay = mps[j]->getWDelay();
        for (j++; j <= ((MParam0 *)mmp2[i][1])->sn; j++) {
            if (mps[j]->getAddr() < m.addr)
                ;//qWarning("warning: initMMParam MRegRWInfo error.");
            else {
                int count = mps[j]->getAddr() - (m.addr + m.count) + mps[j]->getCount();
                if (count <= mps[j]->getCount() && count + m.count <= 125) {
                    if (count > 0)
                        m.count += count;
                    if (m.rdelay < mps[j]->getRDelay())
                        m.rdelay = mps[j]->getRDelay();
                    if (m.wdelay < mps[j]->getWDelay())
                        m.wdelay = mps[j]->getWDelay();
                    continue;
                }
            }
            list.append(m);
            m.addr = mps[j]->getAddr();
            m.count = mps[j]->getCount();
            m.rdelay = mps[j]->getRDelay();
            m.wdelay = mps[j]->getWDelay();
        }
        list.append(m);
    }

    std::sort(list.begin(), list.end(), lessThan);
    MRegRWInfo reg;
    reg.addr = list.first().addr;
    reg.count = 0;
    reg.rdelay = 0;
    reg.wdelay = 0;
    foreach (const MRegRWInfo &m, list) {
        if (reg.addr + reg.count > m.addr)
            qWarning("warning: initMMParam MRegRWInfo register conflict.");
        if (reg.addr + reg.count == m.addr && reg.count + m.count <= 125) {
            reg.count += m.count;
            if (reg.rdelay < m.rdelay)
                reg.rdelay = m.rdelay;
            if (reg.wdelay < m.wdelay)
                reg.wdelay = m.wdelay;
            continue;
        }
        mmp.append(reg);
        reg.addr = m.addr;
        reg.count = m.count;
        reg.rdelay = m.rdelay;
        reg.wdelay = m.wdelay;
    }
    mmp.append(reg);
}
