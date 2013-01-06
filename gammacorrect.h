#ifndef GAMMACORRECT_H
#define GAMMACORRECT_H

class GammaCorrect
{
public:
    GammaCorrect();

    inline static int correct(int iColourComponentValue) {
        return iGammaCorrection[iColourComponentValue];
    }

private:
    static const int iGammaCorrection[];
};

const int GammaCorrect::iGammaCorrection[] = {
        0,
        0,
        0,
        0,
        0,
        1,
        1,
        1,
        2,
        3,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        11,
        12,
        13,
        15,
        17,
        19,
        21,
        23,
        25,
        27,
        29,
        32,
        34,
        37,
        40,
        42,
        45,
        48,
        52,
        55,
        58,
        62,
        66,
        69,
        73,
        77,
        81,
        85,
        90,
        94,
        99,
        104,
        108,
        113,
        118,
        123,
        129,
        134,
        140,
        145,
        151,
        157,
        163,
        169,
        175,
        182,
        188,
        195,
        202,
        209,
        216,
        223,
        230,
        237,
        245,
        253,
        260,
        268,
        276,
        284,
        293,
        301,
        310,
        318,
        327,
        336,
        345,
        355,
        364,
        373,
        383,
        393,
        403,
        413,
        423,
        433,
        444,
        454,
        465,
        476,
        487,
        498,
        509,
        520,
        532,
        543,
        555,
        567,
        579,
        591,
        604,
        616,
        629,
        642,
        655,
        668,
        681,
        694,
        708,
        721,
        735,
        749,
        763,
        777,
        791,
        806,
        820,
        835,
        850,
        865,
        880,
        896,
        911,
        927,
        942,
        958,
        974,
        991,
        1007,
        1023,
        1040,
        1057,
        1074,
        1091,
        1108,
        1125,
        1143,
        1161,
        1178,
        1196,
        1214,
        1233,
        1251,
        1270,
        1288,
        1307,
        1326,
        1345,
        1365,
        1384,
        1404,
        1423,
        1443,
        1463,
        1484,
        1504,
        1524,
        1545,
        1566,
        1587,
        1608,
        1629,
        1651,
        1672,
        1694,
        1716,
        1738,
        1760,
        1782,
        1805,
        1827,
        1850,
        1873,
        1896,
        1919,
        1943,
        1966,
        1990,
        2014,
        2038,
        2062,
        2087,
        2111,
        2136,
        2160,
        2185,
        2211,
        2236,
        2261,
        2287,
        2313,
        2338,
        2365,
        2391,
        2417,
        2444,
        2470,
        2497,
        2524,
        2551,
        2579,
        2606,
        2634,
        2662,
        2690,
        2718,
        2746,
        2774,
        2803,
        2832,
        2861,
        2890,
        2919,
        2949,
        2978,
        3008,
        3038,
        3068,
        3098,
        3128,
        3159,
        3190,
        3220,
        3251,
        3283,
        3314,
        3345,
        3377,
        3409,
        3441,
        3473,
        3505,
        3538,
        3571,
        3603,
        3636,
        3669,
        3703,
        3736,
        3770,
        3804,
        3838,
        3872,
        3906,
        3941,
        3975,
        4010,
        4045,
        4080 };

#endif // GAMMACORRECT_H
