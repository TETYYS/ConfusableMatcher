#include "ConfusableMatcher.h"

const std::vector<std::pair<size_t, size_t>> Ranges = {
	std::pair<size_t, size_t>(0, 40),
	std::pair<size_t, size_t>(42, 47),
	std::pair<size_t, size_t>(58, 64),
	std::pair<size_t, size_t>(91, 96),
	std::pair<size_t, size_t>(123, 127),
	std::pair<size_t, size_t>(128, 169),
	std::pair<size_t, size_t>(171, 177),
	std::pair<size_t, size_t>(180, 180),
	std::pair<size_t, size_t>(182, 184),
	std::pair<size_t, size_t>(187, 187),
	std::pair<size_t, size_t>(191, 191),
	std::pair<size_t, size_t>(215, 215),
	std::pair<size_t, size_t>(247, 247),
	std::pair<size_t, size_t>(706, 709),
	std::pair<size_t, size_t>(722, 735),
	std::pair<size_t, size_t>(741, 747),
	std::pair<size_t, size_t>(749, 749),
	std::pair<size_t, size_t>(751, 879),
	std::pair<size_t, size_t>(885, 885),
	std::pair<size_t, size_t>(894, 894),
	std::pair<size_t, size_t>(900, 901),
	std::pair<size_t, size_t>(903, 903),
	std::pair<size_t, size_t>(1014, 1014),
	std::pair<size_t, size_t>(1154, 1161),
	std::pair<size_t, size_t>(1370, 1375),
	std::pair<size_t, size_t>(1417, 1418),
	std::pair<size_t, size_t>(1421, 1423),
	std::pair<size_t, size_t>(1425, 1479),
	std::pair<size_t, size_t>(1523, 1524),
	std::pair<size_t, size_t>(1536, 1564),
	std::pair<size_t, size_t>(1566, 1567),
	std::pair<size_t, size_t>(1611, 1631),
	std::pair<size_t, size_t>(1642, 1645),
	std::pair<size_t, size_t>(1648, 1648),
	std::pair<size_t, size_t>(1748, 1748),
	std::pair<size_t, size_t>(1750, 1764),
	std::pair<size_t, size_t>(1767, 1773),
	std::pair<size_t, size_t>(1789, 1790),
	std::pair<size_t, size_t>(1792, 1805),
	std::pair<size_t, size_t>(1807, 1807),
	std::pair<size_t, size_t>(1809, 1809),
	std::pair<size_t, size_t>(1840, 1866),
	std::pair<size_t, size_t>(1958, 1968),
	std::pair<size_t, size_t>(2027, 2035),
	std::pair<size_t, size_t>(2038, 2041),
	std::pair<size_t, size_t>(2045, 2047),
	std::pair<size_t, size_t>(2070, 2073),
	std::pair<size_t, size_t>(2075, 2083),
	std::pair<size_t, size_t>(2085, 2087),
	std::pair<size_t, size_t>(2089, 2093),
	std::pair<size_t, size_t>(2096, 2110),
	std::pair<size_t, size_t>(2137, 2139),
	std::pair<size_t, size_t>(2142, 2142),
	std::pair<size_t, size_t>(2259, 2307),
	std::pair<size_t, size_t>(2362, 2364),
	std::pair<size_t, size_t>(2366, 2383),
	std::pair<size_t, size_t>(2385, 2391),
	std::pair<size_t, size_t>(2402, 2405),
	std::pair<size_t, size_t>(2416, 2416),
	std::pair<size_t, size_t>(2433, 2435),
	std::pair<size_t, size_t>(2492, 2492),
	std::pair<size_t, size_t>(2494, 2500),
	std::pair<size_t, size_t>(2503, 2504),
	std::pair<size_t, size_t>(2507, 2509),
	std::pair<size_t, size_t>(2519, 2519),
	std::pair<size_t, size_t>(2530, 2531),
	std::pair<size_t, size_t>(2546, 2547),
	std::pair<size_t, size_t>(2554, 2555),
	std::pair<size_t, size_t>(2557, 2558),
	std::pair<size_t, size_t>(2561, 2563),
	std::pair<size_t, size_t>(2620, 2620),
	std::pair<size_t, size_t>(2622, 2626),
	std::pair<size_t, size_t>(2631, 2632),
	std::pair<size_t, size_t>(2635, 2637),
	std::pair<size_t, size_t>(2641, 2641),
	std::pair<size_t, size_t>(2672, 2673),
	std::pair<size_t, size_t>(2677, 2678),
	std::pair<size_t, size_t>(2689, 2691),
	std::pair<size_t, size_t>(2748, 2748),
	std::pair<size_t, size_t>(2750, 2757),
	std::pair<size_t, size_t>(2759, 2761),
	std::pair<size_t, size_t>(2763, 2765),
	std::pair<size_t, size_t>(2786, 2787),
	std::pair<size_t, size_t>(2800, 2801),
	std::pair<size_t, size_t>(2810, 2815),
	std::pair<size_t, size_t>(2817, 2819),
	std::pair<size_t, size_t>(2876, 2876),
	std::pair<size_t, size_t>(2878, 2884),
	std::pair<size_t, size_t>(2887, 2888),
	std::pair<size_t, size_t>(2891, 2893),
	std::pair<size_t, size_t>(2901, 2903),
	std::pair<size_t, size_t>(2914, 2915),
	std::pair<size_t, size_t>(2928, 2928),
	std::pair<size_t, size_t>(2946, 2946),
	std::pair<size_t, size_t>(3006, 3010),
	std::pair<size_t, size_t>(3014, 3016),
	std::pair<size_t, size_t>(3018, 3021),
	std::pair<size_t, size_t>(3031, 3031),
	std::pair<size_t, size_t>(3059, 3066),
	std::pair<size_t, size_t>(3072, 3076),
	std::pair<size_t, size_t>(3134, 3140),
	std::pair<size_t, size_t>(3142, 3144),
	std::pair<size_t, size_t>(3146, 3149),
	std::pair<size_t, size_t>(3157, 3158),
	std::pair<size_t, size_t>(3170, 3171),
	std::pair<size_t, size_t>(3191, 3191),
	std::pair<size_t, size_t>(3199, 3199),
	std::pair<size_t, size_t>(3201, 3204),
	std::pair<size_t, size_t>(3260, 3260),
	std::pair<size_t, size_t>(3262, 3268),
	std::pair<size_t, size_t>(3270, 3272),
	std::pair<size_t, size_t>(3274, 3277),
	std::pair<size_t, size_t>(3285, 3286),
	std::pair<size_t, size_t>(3298, 3299),
	std::pair<size_t, size_t>(3328, 3331),
	std::pair<size_t, size_t>(3387, 3388),
	std::pair<size_t, size_t>(3390, 3396),
	std::pair<size_t, size_t>(3398, 3400),
	std::pair<size_t, size_t>(3402, 3405),
	std::pair<size_t, size_t>(3407, 3407),
	std::pair<size_t, size_t>(3415, 3415),
	std::pair<size_t, size_t>(3426, 3427),
	std::pair<size_t, size_t>(3449, 3449),
	std::pair<size_t, size_t>(3457, 3459),
	std::pair<size_t, size_t>(3530, 3530),
	std::pair<size_t, size_t>(3535, 3540),
	std::pair<size_t, size_t>(3542, 3542),
	std::pair<size_t, size_t>(3544, 3551),
	std::pair<size_t, size_t>(3570, 3572),
	std::pair<size_t, size_t>(3633, 3633),
	std::pair<size_t, size_t>(3636, 3642),
	std::pair<size_t, size_t>(3647, 3647),
	std::pair<size_t, size_t>(3655, 3663),
	std::pair<size_t, size_t>(3674, 3675),
	std::pair<size_t, size_t>(3761, 3761),
	std::pair<size_t, size_t>(3764, 3772),
	std::pair<size_t, size_t>(3784, 3789),
	std::pair<size_t, size_t>(3841, 3871),
	std::pair<size_t, size_t>(3892, 3903),
	std::pair<size_t, size_t>(3953, 3975),
	std::pair<size_t, size_t>(3981, 3991),
	std::pair<size_t, size_t>(3993, 4028),
	std::pair<size_t, size_t>(4030, 4044),
	std::pair<size_t, size_t>(4046, 4058),
	std::pair<size_t, size_t>(4139, 4158),
	std::pair<size_t, size_t>(4170, 4175),
	std::pair<size_t, size_t>(4182, 4185),
	std::pair<size_t, size_t>(4190, 4192),
	std::pair<size_t, size_t>(4194, 4196),
	std::pair<size_t, size_t>(4199, 4205),
	std::pair<size_t, size_t>(4209, 4212),
	std::pair<size_t, size_t>(4226, 4237),
	std::pair<size_t, size_t>(4239, 4239),
	std::pair<size_t, size_t>(4250, 4255),
	std::pair<size_t, size_t>(4347, 4347),
	std::pair<size_t, size_t>(4957, 4968),
	std::pair<size_t, size_t>(5008, 5017),
	std::pair<size_t, size_t>(5120, 5120),
	std::pair<size_t, size_t>(5741, 5742),
	std::pair<size_t, size_t>(5760, 5760),
	std::pair<size_t, size_t>(5787, 5788),
	std::pair<size_t, size_t>(5867, 5869),
	std::pair<size_t, size_t>(5906, 5908),
	std::pair<size_t, size_t>(5938, 5942),
	std::pair<size_t, size_t>(5970, 5971),
	std::pair<size_t, size_t>(6002, 6003),
	std::pair<size_t, size_t>(6068, 6102),
	std::pair<size_t, size_t>(6104, 6107),
	std::pair<size_t, size_t>(6109, 6109),
	std::pair<size_t, size_t>(6144, 6158),
	std::pair<size_t, size_t>(6277, 6278),
	std::pair<size_t, size_t>(6313, 6313),
	std::pair<size_t, size_t>(6432, 6443),
	std::pair<size_t, size_t>(6448, 6459),
	std::pair<size_t, size_t>(6464, 6464),
	std::pair<size_t, size_t>(6468, 6469),
	std::pair<size_t, size_t>(6622, 6655),
	std::pair<size_t, size_t>(6679, 6683),
	std::pair<size_t, size_t>(6686, 6687),
	std::pair<size_t, size_t>(6741, 6750),
	std::pair<size_t, size_t>(6752, 6780),
	std::pair<size_t, size_t>(6783, 6783),
	std::pair<size_t, size_t>(6816, 6822),
	std::pair<size_t, size_t>(6824, 6829),
	std::pair<size_t, size_t>(6832, 6848),
	std::pair<size_t, size_t>(6912, 6916),
	std::pair<size_t, size_t>(6964, 6980),
	std::pair<size_t, size_t>(7002, 7036),
	std::pair<size_t, size_t>(7040, 7042),
	std::pair<size_t, size_t>(7073, 7085),
	std::pair<size_t, size_t>(7142, 7155),
	std::pair<size_t, size_t>(7164, 7167),
	std::pair<size_t, size_t>(7204, 7223),
	std::pair<size_t, size_t>(7227, 7231),
	std::pair<size_t, size_t>(7294, 7295),
	std::pair<size_t, size_t>(7360, 7367),
	std::pair<size_t, size_t>(7376, 7400),
	std::pair<size_t, size_t>(7405, 7405),
	std::pair<size_t, size_t>(7412, 7412),
	std::pair<size_t, size_t>(7415, 7417),
	std::pair<size_t, size_t>(7616, 7673),
	std::pair<size_t, size_t>(7675, 7679),
	std::pair<size_t, size_t>(8125, 8125),
	std::pair<size_t, size_t>(8127, 8129),
	std::pair<size_t, size_t>(8141, 8143),
	std::pair<size_t, size_t>(8157, 8159),
	std::pair<size_t, size_t>(8173, 8175),
	std::pair<size_t, size_t>(8189, 8190),
	std::pair<size_t, size_t>(8192, 8292),
	std::pair<size_t, size_t>(8294, 8303),
	std::pair<size_t, size_t>(8314, 8318),
	std::pair<size_t, size_t>(8330, 8334),
	std::pair<size_t, size_t>(8352, 8383),
	std::pair<size_t, size_t>(8400, 8432),
	std::pair<size_t, size_t>(8448, 8449),
	std::pair<size_t, size_t>(8451, 8454),
	std::pair<size_t, size_t>(8456, 8457),
	std::pair<size_t, size_t>(8468, 8468),
	std::pair<size_t, size_t>(8470, 8472),
	std::pair<size_t, size_t>(8478, 8483),
	std::pair<size_t, size_t>(8485, 8485),
	std::pair<size_t, size_t>(8487, 8487),
	std::pair<size_t, size_t>(8489, 8489),
	std::pair<size_t, size_t>(8494, 8494),
	std::pair<size_t, size_t>(8506, 8507),
	std::pair<size_t, size_t>(8512, 8516),
	std::pair<size_t, size_t>(8522, 8525),
	std::pair<size_t, size_t>(8527, 8527),
	std::pair<size_t, size_t>(8586, 8587),
	std::pair<size_t, size_t>(8592, 9254),
	std::pair<size_t, size_t>(9280, 9290),
	std::pair<size_t, size_t>(9372, 9449),
	std::pair<size_t, size_t>(9472, 10101),
	std::pair<size_t, size_t>(10132, 11123),
	std::pair<size_t, size_t>(11126, 11157),
	std::pair<size_t, size_t>(11159, 11263),
	std::pair<size_t, size_t>(11493, 11498),
	std::pair<size_t, size_t>(11503, 11505),
	std::pair<size_t, size_t>(11513, 11516),
	std::pair<size_t, size_t>(11518, 11519),
	std::pair<size_t, size_t>(11632, 11632),
	std::pair<size_t, size_t>(11647, 11647),
	std::pair<size_t, size_t>(11744, 11822),
	std::pair<size_t, size_t>(11824, 11858),
	std::pair<size_t, size_t>(11904, 11929),
	std::pair<size_t, size_t>(11931, 12019),
	std::pair<size_t, size_t>(12032, 12245),
	std::pair<size_t, size_t>(12272, 12283),
	std::pair<size_t, size_t>(12288, 12292),
	std::pair<size_t, size_t>(12296, 12320),
	std::pair<size_t, size_t>(12330, 12336),
	std::pair<size_t, size_t>(12342, 12343),
	std::pair<size_t, size_t>(12349, 12351),
	std::pair<size_t, size_t>(12441, 12444),
	std::pair<size_t, size_t>(12448, 12448),
	std::pair<size_t, size_t>(12539, 12539),
	std::pair<size_t, size_t>(12688, 12689),
	std::pair<size_t, size_t>(12694, 12703),
	std::pair<size_t, size_t>(12736, 12771),
	std::pair<size_t, size_t>(12800, 12830),
	std::pair<size_t, size_t>(12842, 12871),
	std::pair<size_t, size_t>(12880, 12880),
	std::pair<size_t, size_t>(12896, 12927),
	std::pair<size_t, size_t>(12938, 12976),
	std::pair<size_t, size_t>(12992, 13311),
	std::pair<size_t, size_t>(19904, 19967),
	std::pair<size_t, size_t>(42128, 42182),
	std::pair<size_t, size_t>(42238, 42239),
	std::pair<size_t, size_t>(42509, 42511),
	std::pair<size_t, size_t>(42607, 42622),
	std::pair<size_t, size_t>(42654, 42655),
	std::pair<size_t, size_t>(42736, 42743),
	std::pair<size_t, size_t>(42752, 42774),
	std::pair<size_t, size_t>(42784, 42785),
	std::pair<size_t, size_t>(42889, 42890),
	std::pair<size_t, size_t>(43010, 43010),
	std::pair<size_t, size_t>(43014, 43014),
	std::pair<size_t, size_t>(43019, 43019),
	std::pair<size_t, size_t>(43043, 43052),
	std::pair<size_t, size_t>(43062, 43065),
	std::pair<size_t, size_t>(43124, 43127),
	std::pair<size_t, size_t>(43136, 43137),
	std::pair<size_t, size_t>(43188, 43205),
	std::pair<size_t, size_t>(43214, 43215),
	std::pair<size_t, size_t>(43232, 43249),
	std::pair<size_t, size_t>(43256, 43258),
	std::pair<size_t, size_t>(43260, 43260),
	std::pair<size_t, size_t>(43263, 43263),
	std::pair<size_t, size_t>(43302, 43311),
	std::pair<size_t, size_t>(43335, 43347),
	std::pair<size_t, size_t>(43359, 43359),
	std::pair<size_t, size_t>(43392, 43395),
	std::pair<size_t, size_t>(43443, 43469),
	std::pair<size_t, size_t>(43486, 43487),
	std::pair<size_t, size_t>(43493, 43493),
	std::pair<size_t, size_t>(43561, 43574),
	std::pair<size_t, size_t>(43587, 43587),
	std::pair<size_t, size_t>(43596, 43597),
	std::pair<size_t, size_t>(43612, 43615),
	std::pair<size_t, size_t>(43639, 43641),
	std::pair<size_t, size_t>(43643, 43645),
	std::pair<size_t, size_t>(43696, 43696),
	std::pair<size_t, size_t>(43698, 43700),
	std::pair<size_t, size_t>(43703, 43704),
	std::pair<size_t, size_t>(43710, 43711),
	std::pair<size_t, size_t>(43713, 43713),
	std::pair<size_t, size_t>(43742, 43743),
	std::pair<size_t, size_t>(43755, 43761),
	std::pair<size_t, size_t>(43765, 43766),
	std::pair<size_t, size_t>(43867, 43867),
	std::pair<size_t, size_t>(43882, 43883),
	std::pair<size_t, size_t>(44003, 44013),
	std::pair<size_t, size_t>(55296, 55296),
	std::pair<size_t, size_t>(56191, 56192),
	std::pair<size_t, size_t>(56319, 56320),
	std::pair<size_t, size_t>(57343, 57344),
	std::pair<size_t, size_t>(63743, 63743),
	std::pair<size_t, size_t>(64286, 64286),
	std::pair<size_t, size_t>(64297, 64297),
	std::pair<size_t, size_t>(64434, 64449),
	std::pair<size_t, size_t>(64830, 64831),
	std::pair<size_t, size_t>(65020, 65021),
	std::pair<size_t, size_t>(65024, 65049),
	std::pair<size_t, size_t>(65056, 65106),
	std::pair<size_t, size_t>(65108, 65126),
	std::pair<size_t, size_t>(65128, 65131),
	std::pair<size_t, size_t>(65279, 65279),
	std::pair<size_t, size_t>(65281, 65295),
	std::pair<size_t, size_t>(65306, 65312),
	std::pair<size_t, size_t>(65339, 65344),
	std::pair<size_t, size_t>(65371, 65381),
	std::pair<size_t, size_t>(65504, 65510),
	std::pair<size_t, size_t>(65512, 65518),
	std::pair<size_t, size_t>(65529, 65533),
	std::pair<size_t, size_t>(65792, 65794),
	std::pair<size_t, size_t>(65847, 65855),
	std::pair<size_t, size_t>(65913, 65929),
	std::pair<size_t, size_t>(65932, 65934),
	std::pair<size_t, size_t>(65936, 65948),
	std::pair<size_t, size_t>(65952, 65952),
	std::pair<size_t, size_t>(66000, 66045),
	std::pair<size_t, size_t>(66272, 66272),
	std::pair<size_t, size_t>(66422, 66426),
	std::pair<size_t, size_t>(66463, 66463),
	std::pair<size_t, size_t>(66512, 66512),
	std::pair<size_t, size_t>(66927, 66927),
	std::pair<size_t, size_t>(67671, 67671),
	std::pair<size_t, size_t>(67703, 67704),
	std::pair<size_t, size_t>(67871, 67871),
	std::pair<size_t, size_t>(67903, 67903),
	std::pair<size_t, size_t>(68097, 68099),
	std::pair<size_t, size_t>(68101, 68102),
	std::pair<size_t, size_t>(68108, 68111),
	std::pair<size_t, size_t>(68152, 68154),
	std::pair<size_t, size_t>(68159, 68159),
	std::pair<size_t, size_t>(68176, 68184),
	std::pair<size_t, size_t>(68223, 68223),
	std::pair<size_t, size_t>(68296, 68296),
	std::pair<size_t, size_t>(68325, 68326),
	std::pair<size_t, size_t>(68336, 68342),
	std::pair<size_t, size_t>(68409, 68415),
	std::pair<size_t, size_t>(68505, 68508),
	std::pair<size_t, size_t>(68900, 68903),
	std::pair<size_t, size_t>(69291, 69293),
	std::pair<size_t, size_t>(69446, 69456),
	std::pair<size_t, size_t>(69461, 69465),
	std::pair<size_t, size_t>(69632, 69634),
	std::pair<size_t, size_t>(69688, 69709),
	std::pair<size_t, size_t>(69759, 69762),
	std::pair<size_t, size_t>(69808, 69825),
	std::pair<size_t, size_t>(69837, 69837),
	std::pair<size_t, size_t>(69888, 69890),
	std::pair<size_t, size_t>(69927, 69940),
	std::pair<size_t, size_t>(69952, 69955),
	std::pair<size_t, size_t>(69957, 69958),
	std::pair<size_t, size_t>(70003, 70005),
	std::pair<size_t, size_t>(70016, 70018),
	std::pair<size_t, size_t>(70067, 70080),
	std::pair<size_t, size_t>(70085, 70095),
	std::pair<size_t, size_t>(70107, 70107),
	std::pair<size_t, size_t>(70109, 70111),
	std::pair<size_t, size_t>(70188, 70206),
	std::pair<size_t, size_t>(70313, 70313),
	std::pair<size_t, size_t>(70367, 70378),
	std::pair<size_t, size_t>(70400, 70403),
	std::pair<size_t, size_t>(70459, 70460),
	std::pair<size_t, size_t>(70462, 70468),
	std::pair<size_t, size_t>(70471, 70472),
	std::pair<size_t, size_t>(70475, 70477),
	std::pair<size_t, size_t>(70487, 70487),
	std::pair<size_t, size_t>(70498, 70499),
	std::pair<size_t, size_t>(70502, 70508),
	std::pair<size_t, size_t>(70512, 70516),
	std::pair<size_t, size_t>(70709, 70726),
	std::pair<size_t, size_t>(70731, 70735),
	std::pair<size_t, size_t>(70746, 70747),
	std::pair<size_t, size_t>(70749, 70750),
	std::pair<size_t, size_t>(70832, 70851),
	std::pair<size_t, size_t>(70854, 70854),
	std::pair<size_t, size_t>(71087, 71093),
	std::pair<size_t, size_t>(71096, 71127),
	std::pair<size_t, size_t>(71132, 71133),
	std::pair<size_t, size_t>(71216, 71235),
	std::pair<size_t, size_t>(71264, 71276),
	std::pair<size_t, size_t>(71339, 71351),
	std::pair<size_t, size_t>(71453, 71467),
	std::pair<size_t, size_t>(71484, 71487),
	std::pair<size_t, size_t>(71724, 71739),
	std::pair<size_t, size_t>(71984, 71989),
	std::pair<size_t, size_t>(71991, 71992),
	std::pair<size_t, size_t>(71995, 71998),
	std::pair<size_t, size_t>(72000, 72000),
	std::pair<size_t, size_t>(72002, 72006),
	std::pair<size_t, size_t>(72145, 72151),
	std::pair<size_t, size_t>(72154, 72160),
	std::pair<size_t, size_t>(72162, 72162),
	std::pair<size_t, size_t>(72164, 72164),
	std::pair<size_t, size_t>(72193, 72202),
	std::pair<size_t, size_t>(72243, 72249),
	std::pair<size_t, size_t>(72251, 72263),
	std::pair<size_t, size_t>(72273, 72283),
	std::pair<size_t, size_t>(72330, 72348),
	std::pair<size_t, size_t>(72350, 72354),
	std::pair<size_t, size_t>(72751, 72758),
	std::pair<size_t, size_t>(72760, 72767),
	std::pair<size_t, size_t>(72769, 72773),
	std::pair<size_t, size_t>(72816, 72817),
	std::pair<size_t, size_t>(72850, 72871),
	std::pair<size_t, size_t>(72873, 72886),
	std::pair<size_t, size_t>(73009, 73014),
	std::pair<size_t, size_t>(73018, 73018),
	std::pair<size_t, size_t>(73020, 73021),
	std::pair<size_t, size_t>(73023, 73029),
	std::pair<size_t, size_t>(73031, 73031),
	std::pair<size_t, size_t>(73098, 73102),
	std::pair<size_t, size_t>(73104, 73105),
	std::pair<size_t, size_t>(73107, 73111),
	std::pair<size_t, size_t>(73459, 73464),
	std::pair<size_t, size_t>(73685, 73713),
	std::pair<size_t, size_t>(73727, 73727),
	std::pair<size_t, size_t>(74864, 74868),
	std::pair<size_t, size_t>(78896, 78904),
	std::pair<size_t, size_t>(92782, 92783),
	std::pair<size_t, size_t>(92912, 92917),
	std::pair<size_t, size_t>(92976, 92991),
	std::pair<size_t, size_t>(92996, 92997),
	std::pair<size_t, size_t>(93847, 93850),
	std::pair<size_t, size_t>(94031, 94031),
	std::pair<size_t, size_t>(94033, 94087),
	std::pair<size_t, size_t>(94095, 94098),
	std::pair<size_t, size_t>(94178, 94178),
	std::pair<size_t, size_t>(94180, 94180),
	std::pair<size_t, size_t>(94192, 94193),
	std::pair<size_t, size_t>(113820, 113827),
	std::pair<size_t, size_t>(118784, 119029),
	std::pair<size_t, size_t>(119040, 119078),
	std::pair<size_t, size_t>(119081, 119272),
	std::pair<size_t, size_t>(119296, 119365),
	std::pair<size_t, size_t>(119552, 119638),
	std::pair<size_t, size_t>(120513, 120513),
	std::pair<size_t, size_t>(120539, 120539),
	std::pair<size_t, size_t>(120571, 120571),
	std::pair<size_t, size_t>(120597, 120597),
	std::pair<size_t, size_t>(120629, 120629),
	std::pair<size_t, size_t>(120655, 120655),
	std::pair<size_t, size_t>(120687, 120687),
	std::pair<size_t, size_t>(120713, 120713),
	std::pair<size_t, size_t>(120745, 120745),
	std::pair<size_t, size_t>(120771, 120771),
	std::pair<size_t, size_t>(120832, 121483),
	std::pair<size_t, size_t>(121499, 121503),
	std::pair<size_t, size_t>(121505, 121519),
	std::pair<size_t, size_t>(122880, 122886),
	std::pair<size_t, size_t>(122888, 122904),
	std::pair<size_t, size_t>(122907, 122913),
	std::pair<size_t, size_t>(122915, 122916),
	std::pair<size_t, size_t>(122918, 122922),
	std::pair<size_t, size_t>(123184, 123190),
	std::pair<size_t, size_t>(123215, 123215),
	std::pair<size_t, size_t>(123628, 123631),
	std::pair<size_t, size_t>(123647, 123647),
	std::pair<size_t, size_t>(125136, 125142),
	std::pair<size_t, size_t>(125252, 125258),
	std::pair<size_t, size_t>(125278, 125279),
	std::pair<size_t, size_t>(126124, 126124),
	std::pair<size_t, size_t>(126128, 126128),
	std::pair<size_t, size_t>(126254, 126254),
	std::pair<size_t, size_t>(126704, 126705),
	std::pair<size_t, size_t>(126976, 127019),
	std::pair<size_t, size_t>(127024, 127123),
	std::pair<size_t, size_t>(127136, 127150),
	std::pair<size_t, size_t>(127153, 127167),
	std::pair<size_t, size_t>(127169, 127183),
	std::pair<size_t, size_t>(127185, 127221),
	std::pair<size_t, size_t>(127245, 127405),
	std::pair<size_t, size_t>(127462, 127490),
	std::pair<size_t, size_t>(127504, 127547),
	std::pair<size_t, size_t>(127552, 127560),
	std::pair<size_t, size_t>(127568, 127569),
	std::pair<size_t, size_t>(127584, 127589),
	std::pair<size_t, size_t>(127744, 128727),
	std::pair<size_t, size_t>(128736, 128748),
	std::pair<size_t, size_t>(128752, 128764),
	std::pair<size_t, size_t>(128768, 128883),
	std::pair<size_t, size_t>(128896, 128984),
	std::pair<size_t, size_t>(128992, 129003),
	std::pair<size_t, size_t>(129024, 129035),
	std::pair<size_t, size_t>(129040, 129095),
	std::pair<size_t, size_t>(129104, 129113),
	std::pair<size_t, size_t>(129120, 129159),
	std::pair<size_t, size_t>(129168, 129197),
	std::pair<size_t, size_t>(129200, 129201),
	std::pair<size_t, size_t>(129280, 129400),
	std::pair<size_t, size_t>(129402, 129483),
	std::pair<size_t, size_t>(129485, 129619),
	std::pair<size_t, size_t>(129632, 129645),
	std::pair<size_t, size_t>(129648, 129652),
	std::pair<size_t, size_t>(129656, 129658),
	std::pair<size_t, size_t>(129664, 129670),
	std::pair<size_t, size_t>(129680, 129704),
	std::pair<size_t, size_t>(129712, 129718),
	std::pair<size_t, size_t>(129728, 129730),
	std::pair<size_t, size_t>(129744, 129750),
	std::pair<size_t, size_t>(129792, 129938),
	std::pair<size_t, size_t>(129940, 129994),
	std::pair<size_t, size_t>(917505, 917505),
	std::pair<size_t, size_t>(917536, 917631),
	std::pair<size_t, size_t>(917760, 917999),
	std::pair<size_t, size_t>(983040, 983040),
	std::pair<size_t, size_t>(1048573, 1048573),
	std::pair<size_t, size_t>(1048576, 1048576),
	std::pair<size_t, size_t>(1114109, 1114109)
};

void ConfusableMatcher::Init()
{
	for (auto range : Ranges) {
		for (auto x = range.first;x <= range.second;x++) {
			ConfusableMatcher::WordBoundaries.set(x, true);
		}
	}
}