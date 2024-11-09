#include <stdio.h>
#include "tests.c"

#define FOUT stdout

typedef struct {
	const char* name;
	_Bool(*test)();
} testcase;

int main()
{
	testcase cases[441];

	if (!setup()) {
		printf("PRE failed to complete successfully. Exiting.\n");
		return 1;
	}

	cases[0].name = "retrieve_onerule_char_one";
	cases[0].test = retrieve_onerule_char_one;

	cases[1].name = "retrieve_onerule_char_many";
	cases[1].test = retrieve_onerule_char_many;

	cases[2].name = "retrieve_onerule_binval_one";
	cases[2].test = retrieve_onerule_binval_one;

	cases[3].name = "retrieve_onerule_binval_many";
	cases[3].test = retrieve_onerule_binval_many;

	cases[4].name = "retrieve_onerule_binval_range_min";
	cases[4].test = retrieve_onerule_binval_range_min;

	cases[5].name = "retrieve_onerule_binval_range_middle";
	cases[5].test = retrieve_onerule_binval_range_middle;

	cases[6].name = "retrieve_onerule_binval_range_max";
	cases[6].test = retrieve_onerule_binval_range_max;

	cases[7].name = "retrieve_onerule_decval_one";
	cases[7].test = retrieve_onerule_decval_one;

	cases[8].name = "retrieve_onerule_decval_many";
	cases[8].test = retrieve_onerule_decval_many;

	cases[9].name = "retrieve_onerule_decval_range_min";
	cases[9].test = retrieve_onerule_decval_range_min;

	cases[10].name = "retrieve_onerule_decval_range_middle";
	cases[10].test = retrieve_onerule_decval_range_middle;

	cases[11].name = "retrieve_onerule_decval_range_max";
	cases[11].test = retrieve_onerule_decval_range_max;

	cases[12].name = "retrieve_onerule_hexval_one";
	cases[12].test = retrieve_onerule_hexval_one;

	cases[13].name = "retrieve_onerule_hexval_many";
	cases[13].test = retrieve_onerule_hexval_many;

	cases[14].name = "retrieve_onerule_hexval_range_min";
	cases[14].test = retrieve_onerule_hexval_range_min;

	cases[15].name = "retrieve_onerule_hexval_range_middle";
	cases[15].test = retrieve_onerule_hexval_range_middle;

	cases[16].name = "retrieve_onerule_hexval_range_max";
	cases[16].test = retrieve_onerule_hexval_range_max;

	cases[17].name = "retrieve_onerule_prose_one";
	cases[17].test = retrieve_onerule_prose_one;

	cases[18].name = "retrieve_onerule_prose_many";
	cases[18].test = retrieve_onerule_prose_many;

	cases[19].name = "retrieve_onerule_group";
	cases[19].test = retrieve_onerule_group;

	cases[20].name = "retrieve_onerule_group_repeat";
	cases[20].test = retrieve_onerule_group_repeat;

	cases[21].name = "retrieve_onerule_group_repeat_zero";
	cases[21].test = retrieve_onerule_group_repeat_zero;

	cases[22].name = "retrieve_onerule_group_range_min";
	cases[22].test = retrieve_onerule_group_range_min;

	cases[23].name = "retrieve_onerule_group_range_middle";
	cases[23].test = retrieve_onerule_group_range_middle;

	cases[24].name = "retrieve_onerule_group_range_max";
	cases[24].test = retrieve_onerule_group_range_max;

	cases[25].name = "retrieve_onerule_group_range_nested_empty";
	cases[25].test = retrieve_onerule_group_range_nested_empty;

	cases[26].name = "retrieve_onerule_group_range_nested_one";
	cases[26].test = retrieve_onerule_group_range_nested_one;

	cases[27].name = "retrieve_onerule_group_range_nested_some";
	cases[27].test = retrieve_onerule_group_range_nested_some;

	cases[28].name = "retrieve_onerule_group_range_nested_max";
	cases[28].test = retrieve_onerule_group_range_nested_max;

	cases[29].name = "retrieve_onerule_group_minrange_min";
	cases[29].test = retrieve_onerule_group_minrange_min;

	cases[30].name = "retrieve_onerule_group_minrange_some";
	cases[30].test = retrieve_onerule_group_minrange_some;

	cases[31].name = "retrieve_onerule_group_minrange_lots";
	cases[31].test = retrieve_onerule_group_minrange_lots;

	cases[32].name = "retrieve_onerule_group_maxrange_empty";
	cases[32].test = retrieve_onerule_group_maxrange_empty;

	cases[33].name = "retrieve_onerule_group_maxrange_one";
	cases[33].test = retrieve_onerule_group_maxrange_one;

	cases[34].name = "retrieve_onerule_group_maxrange_two";
	cases[34].test = retrieve_onerule_group_maxrange_two;

	cases[35].name = "retrieve_onerule_group_maxrange_three";
	cases[35].test = retrieve_onerule_group_maxrange_three;

	cases[36].name = "retrieve_onerule_group_maxrange_max";
	cases[36].test = retrieve_onerule_group_maxrange_max;

	cases[37].name = "retrieve_onerule_group_nested";
	cases[37].test = retrieve_onerule_group_nested;

	cases[38].name = "retrieve_onerule_option_empty";
	cases[38].test = retrieve_onerule_option_empty;

	cases[39].name = "retrieve_onerule_option_nonempty";
	cases[39].test = retrieve_onerule_option_nonempty;

	cases[40].name = "retrieve_onerule_option_nested_empty";
	cases[40].test = retrieve_onerule_option_nested_empty;

	cases[41].name = "retrieve_onerule_option_nested_nonempty";
	cases[41].test = retrieve_onerule_option_nested_nonempty;

	cases[42].name = "retrieve_onerule_option_range_zero";
	cases[42].test = retrieve_onerule_option_range_zero;

	cases[43].name = "retrieve_onerule_option_range_one";
	cases[43].test = retrieve_onerule_option_range_one;

	cases[44].name = "retrieve_onerule_option_range_two";
	cases[44].test = retrieve_onerule_option_range_two;

	cases[45].name = "retrieve_onerule_option_range_three";
	cases[45].test = retrieve_onerule_option_range_three;

	cases[46].name = "retrieve_onerule_option_range_max";
	cases[46].test = retrieve_onerule_option_range_max;

	cases[47].name = "retrieve_onerule_option_range_nested_empty";
	cases[47].test = retrieve_onerule_option_range_nested_empty;

	cases[48].name = "retrieve_onerule_option_range_nested_one";
	cases[48].test = retrieve_onerule_option_range_nested_one;

	cases[49].name = "retrieve_onerule_option_range_nested_some";
	cases[49].test = retrieve_onerule_option_range_nested_some;

	cases[50].name = "retrieve_onerule_option_range_nested_max";
	cases[50].test = retrieve_onerule_option_range_nested_max;

	cases[51].name = "retrieve_onerule_option_minrange_min";
	cases[51].test = retrieve_onerule_option_minrange_min;

	cases[52].name = "retrieve_onerule_option_minrange_some";
	cases[52].test = retrieve_onerule_option_minrange_some;

	cases[53].name = "retrieve_onerule_option_minrange_lots";
	cases[53].test = retrieve_onerule_option_minrange_lots;

	cases[54].name = "retrieve_onerule_option_maxrange_zero";
	cases[54].test = retrieve_onerule_option_maxrange_zero;

	cases[55].name = "retrieve_onerule_option_maxrange_one";
	cases[55].test = retrieve_onerule_option_maxrange_one;

	cases[56].name = "retrieve_onerule_option_maxrange_two";
	cases[56].test = retrieve_onerule_option_maxrange_two;

	cases[57].name = "retrieve_onerule_option_maxrange_three";
	cases[57].test = retrieve_onerule_option_maxrange_three;

	cases[58].name = "retrieve_onerule_option_maxrange_max";
	cases[58].test = retrieve_onerule_option_maxrange_max;

	cases[59].name = "retrieve_onerule_concat_char";
	cases[59].test = retrieve_onerule_concat_char;

	cases[60].name = "retrieve_onerule_concat_char_multiple";
	cases[60].test = retrieve_onerule_concat_char_multiple;

	cases[61].name = "retrieve_onerule_concat_char_repeat_first";
	cases[61].test = retrieve_onerule_concat_char_repeat_first;

	cases[62].name = "retrieve_onerule_concat_char_repeat_middle";
	cases[62].test = retrieve_onerule_concat_char_repeat_middle;

	cases[63].name = "retrieve_onerule_concat_char_repeat_last";
	cases[63].test = retrieve_onerule_concat_char_repeat_last;

	cases[64].name = "retrieve_onerule_concat_char_repeat_all";
	cases[64].test = retrieve_onerule_concat_char_repeat_all;

	cases[65].name = "retrieve_onerule_alt_first";
	cases[65].test = retrieve_onerule_alt_first;

	cases[66].name = "retrieve_onerule_alt_middle";
	cases[66].test = retrieve_onerule_alt_middle;

	cases[67].name = "retrieve_onerule_alt_last";
	cases[67].test = retrieve_onerule_alt_last;

	cases[68].name = "retrieve_onerule_alt_cat_first";
	cases[68].test = retrieve_onerule_alt_cat_first;

	cases[69].name = "retrieve_onerule_alt_cat_middle";
	cases[69].test = retrieve_onerule_alt_cat_middle;

	cases[70].name = "retrieve_onerule_alt_cat_last";
	cases[70].test = retrieve_onerule_alt_cat_last;

	cases[71].name = "retrieve_onerule_cat_alt_variation1";
	cases[71].test = retrieve_onerule_cat_alt_variation1;

	cases[72].name = "retrieve_onerule_cat_alt_variation2";
	cases[72].test = retrieve_onerule_cat_alt_variation2;

	cases[73].name = "retrieve_onerule_alt_cat_outer_repeat_empty";
	cases[73].test = retrieve_onerule_alt_cat_outer_repeat_empty;

	cases[74].name = "retrieve_onerule_alt_cat_outer_repeat";
	cases[74].test = retrieve_onerule_alt_cat_outer_repeat;

	cases[75].name = "retrieve_onerule_alt_cat_outer_repeat2";
	cases[75].test = retrieve_onerule_alt_cat_outer_repeat2;

	cases[76].name = "retrieve_onerule_alt_cat_outer_repeat3";
	cases[76].test = retrieve_onerule_alt_cat_outer_repeat3;

	cases[77].name = "retrieve_onerule_alt_cat_outer_repeat12";
	cases[77].test = retrieve_onerule_alt_cat_outer_repeat12;

	cases[78].name = "retrieve_onerule_alt_cat_outer_repeat21";
	cases[78].test = retrieve_onerule_alt_cat_outer_repeat21;

	cases[79].name = "retrieve_onerule_alt_cat_outer_repeat31";
	cases[79].test = retrieve_onerule_alt_cat_outer_repeat31;

	cases[80].name = "retrieve_onerule_alt_cat_outer_repeat23";
	cases[80].test = retrieve_onerule_alt_cat_outer_repeat23;

	cases[81].name = "retrieve_onerule_alt_cat_outer_repeat32";
	cases[81].test = retrieve_onerule_alt_cat_outer_repeat32;

	cases[82].name = "retrieve_onerule_alt_cat_outer_repeat123";
	cases[82].test = retrieve_onerule_alt_cat_outer_repeat123;

	cases[83].name = "retrieve_onerule_alt_cat_outer_repeat132";
	cases[83].test = retrieve_onerule_alt_cat_outer_repeat132;

	cases[84].name = "retrieve_onerule_alt_cat_outer_repeat213";
	cases[84].test = retrieve_onerule_alt_cat_outer_repeat213;

	cases[85].name = "retrieve_onerule_alt_cat_outer_repeat231";
	cases[85].test = retrieve_onerule_alt_cat_outer_repeat231;

	cases[86].name = "retrieve_onerule_alt_cat_outer_repeat312";
	cases[86].test = retrieve_onerule_alt_cat_outer_repeat312;

	cases[87].name = "retrieve_onerule_alt_cat_outer_repeat321";
	cases[87].test = retrieve_onerule_alt_cat_outer_repeat321;

	cases[88].name = "retrieve_onerule_alt_cat_outer_repeat_mixed";
	cases[88].test = retrieve_onerule_alt_cat_outer_repeat_mixed;

	cases[89].name = "retrieve_onerule_alt_cat_inner_repeat_empty";
	cases[89].test = retrieve_onerule_alt_cat_inner_repeat_empty;

	cases[90].name = "retrieve_onerule_alt_cat_inner_repeat_first_one";
	cases[90].test = retrieve_onerule_alt_cat_inner_repeat_first_one;

	cases[91].name = "retrieve_onerule_alt_cat_inner_repeat_first_two";
	cases[91].test = retrieve_onerule_alt_cat_inner_repeat_first_two;

	cases[92].name = "retrieve_onerule_alt_cat_inner_repeat_first_many";
	cases[92].test = retrieve_onerule_alt_cat_inner_repeat_first_many;

	cases[93].name = "retrieve_onerule_alt_cat_inner_repeat_middle_one";
	cases[93].test = retrieve_onerule_alt_cat_inner_repeat_middle_one;

	cases[94].name = "retrieve_onerule_alt_cat_inner_repeat_middle_two";
	cases[94].test = retrieve_onerule_alt_cat_inner_repeat_middle_two;

	cases[95].name = "retrieve_onerule_alt_cat_inner_repeat_middle_many";
	cases[95].test = retrieve_onerule_alt_cat_inner_repeat_middle_many;

	cases[96].name = "retrieve_onerule_alt_cat_inner_repeat_last_one";
	cases[96].test = retrieve_onerule_alt_cat_inner_repeat_last_one;

	cases[97].name = "retrieve_onerule_alt_cat_inner_repeat_last_two";
	cases[97].test = retrieve_onerule_alt_cat_inner_repeat_last_two;

	cases[98].name = "retrieve_onerule_alt_cat_inner_repeat_last_many";
	cases[98].test = retrieve_onerule_alt_cat_inner_repeat_last_many;

	cases[99].name = "retrieve_onerule_cat_alt_outer_repeat_empty";
	cases[99].test = retrieve_onerule_cat_alt_outer_repeat_empty;

	cases[100].name = "retrieve_onerule_cat_alt_outer_repeat_variation1_one";
	cases[100].test = retrieve_onerule_cat_alt_outer_repeat_variation1_one;

	cases[101].name = "retrieve_onerule_cat_alt_outer_repeat_variation1_two";
	cases[101].test = retrieve_onerule_cat_alt_outer_repeat_variation1_two;

	cases[102].name = "retrieve_onerule_cat_alt_outer_repeat_variation1_many";
	cases[102].test = retrieve_onerule_cat_alt_outer_repeat_variation1_many;

	cases[103].name = "retrieve_onerule_cat_alt_outer_repeat_variation2_one";
	cases[103].test = retrieve_onerule_cat_alt_outer_repeat_variation2_one;

	cases[104].name = "retrieve_onerule_cat_alt_outer_repeat_variation2_two";
	cases[104].test = retrieve_onerule_cat_alt_outer_repeat_variation2_two;

	cases[105].name = "retrieve_onerule_cat_alt_outer_repeat_variation2_many";
	cases[105].test = retrieve_onerule_cat_alt_outer_repeat_variation2_many;

	cases[106].name = "retrieve_onerule_cat_alt_outer_repeat_mixed12";
	cases[106].test = retrieve_onerule_cat_alt_outer_repeat_mixed12;

	cases[107].name = "retrieve_onerule_cat_alt_outer_repeat_mixed21";
	cases[107].test = retrieve_onerule_cat_alt_outer_repeat_mixed21;

	cases[108].name = "retrieve_onerule_cat_alt_outer_repeat_mixed112";
	cases[108].test = retrieve_onerule_cat_alt_outer_repeat_mixed112;

	cases[109].name = "retrieve_onerule_cat_alt_outer_repeat_mixed121";
	cases[109].test = retrieve_onerule_cat_alt_outer_repeat_mixed121;

	cases[110].name = "retrieve_onerule_cat_alt_outer_repeat_mixed122";
	cases[110].test = retrieve_onerule_cat_alt_outer_repeat_mixed122;

	cases[111].name = "retrieve_onerule_cat_alt_outer_repeat_mixed211";
	cases[111].test = retrieve_onerule_cat_alt_outer_repeat_mixed211;

	cases[112].name = "retrieve_onerule_cat_alt_outer_repeat_mixed212";
	cases[112].test = retrieve_onerule_cat_alt_outer_repeat_mixed212;

	cases[113].name = "retrieve_onerule_cat_alt_outer_repeat_mixed221";
	cases[113].test = retrieve_onerule_cat_alt_outer_repeat_mixed221;

	cases[114].name = "retrieve_onerule_cat_alt_outer_repeat_mixed";
	cases[114].test = retrieve_onerule_cat_alt_outer_repeat_mixed;

	cases[115].name = "retrieve_onerule_cat_alt_inner_repeat_empty";
	cases[115].test = retrieve_onerule_cat_alt_inner_repeat_empty;

	cases[116].name = "retrieve_onerule_cat_alt_inner_repeat_first_one";
	cases[116].test = retrieve_onerule_cat_alt_inner_repeat_first_one;

	cases[117].name = "retrieve_onerule_cat_alt_inner_repeat_first_two";
	cases[117].test = retrieve_onerule_cat_alt_inner_repeat_first_two;

	cases[118].name = "retrieve_onerule_cat_alt_inner_repeat_first_many";
	cases[118].test = retrieve_onerule_cat_alt_inner_repeat_first_many;

	cases[119].name = "retrieve_onerule_cat_alt_inner_repeat_middle_variation1_one";
	cases[119].test = retrieve_onerule_cat_alt_inner_repeat_middle_variation1_one;

	cases[120].name = "retrieve_onerule_cat_alt_inner_repeat_middle_variation1_two";
	cases[120].test = retrieve_onerule_cat_alt_inner_repeat_middle_variation1_two;

	cases[121].name = "retrieve_onerule_cat_alt_inner_repeat_middle_variation1_many";
	cases[121].test = retrieve_onerule_cat_alt_inner_repeat_middle_variation1_many;

	cases[122].name = "retrieve_onerule_cat_alt_inner_repeat_middle_variation2_one";
	cases[122].test = retrieve_onerule_cat_alt_inner_repeat_middle_variation2_one;

	cases[123].name = "retrieve_onerule_cat_alt_inner_repeat_middle_variation2_two";
	cases[123].test = retrieve_onerule_cat_alt_inner_repeat_middle_variation2_two;

	cases[124].name = "retrieve_onerule_cat_alt_inner_repeat_middle_variation2_many";
	cases[124].test = retrieve_onerule_cat_alt_inner_repeat_middle_variation2_many;

	cases[125].name = "retrieve_onerule_cat_alt_inner_repeat_last_one";
	cases[125].test = retrieve_onerule_cat_alt_inner_repeat_last_one;

	cases[126].name = "retrieve_onerule_cat_alt_inner_repeat_last_two";
	cases[126].test = retrieve_onerule_cat_alt_inner_repeat_last_two;

	cases[127].name = "retrieve_onerule_cat_alt_inner_repeat_last_many";
	cases[127].test = retrieve_onerule_cat_alt_inner_repeat_last_many;

	cases[128].name = "retrieve_onerule_cat_alt_inner_repeat_12_1_one";
	cases[128].test = retrieve_onerule_cat_alt_inner_repeat_12_1_one;

	cases[129].name = "retrieve_onerule_cat_alt_inner_repeat_12_1_two";
	cases[129].test = retrieve_onerule_cat_alt_inner_repeat_12_1_two;

	cases[130].name = "retrieve_onerule_cat_alt_inner_repeat_12_1_many";
	cases[130].test = retrieve_onerule_cat_alt_inner_repeat_12_1_many;

	cases[131].name = "retrieve_onerule_cat_alt_inner_repeat_12_2_one";
	cases[131].test = retrieve_onerule_cat_alt_inner_repeat_12_2_one;

	cases[132].name = "retrieve_onerule_cat_alt_inner_repeat_12_2_two";
	cases[132].test = retrieve_onerule_cat_alt_inner_repeat_12_2_two;

	cases[133].name = "retrieve_onerule_cat_alt_inner_repeat_12_2_many";
	cases[133].test = retrieve_onerule_cat_alt_inner_repeat_12_2_many;

	cases[134].name = "retrieve_onerule_cat_alt_inner_repeat_13_one";
	cases[134].test = retrieve_onerule_cat_alt_inner_repeat_13_one;

	cases[135].name = "retrieve_onerule_cat_alt_inner_repeat_13_two";
	cases[135].test = retrieve_onerule_cat_alt_inner_repeat_13_two;

	cases[136].name = "retrieve_onerule_cat_alt_inner_repeat_13_many";
	cases[136].test = retrieve_onerule_cat_alt_inner_repeat_13_many;

	cases[137].name = "retrieve_onerule_cat_alt_inner_repeat_mixed_variation1_one";
	cases[137].test = retrieve_onerule_cat_alt_inner_repeat_mixed_variation1_one;

	cases[138].name = "retrieve_onerule_cat_alt_inner_repeat_mixed_variation1_two";
	cases[138].test = retrieve_onerule_cat_alt_inner_repeat_mixed_variation1_two;

	cases[139].name = "retrieve_onerule_cat_alt_inner_repeat_mixed_variation1_many";
	cases[139].test = retrieve_onerule_cat_alt_inner_repeat_mixed_variation1_many;

	cases[140].name = "retrieve_onerule_cat_alt_inner_repeat_mixed_variation2_one";
	cases[140].test = retrieve_onerule_cat_alt_inner_repeat_mixed_variation2_one;

	cases[141].name = "retrieve_onerule_cat_alt_inner_repeat_mixed_variation2_two";
	cases[141].test = retrieve_onerule_cat_alt_inner_repeat_mixed_variation2_two;

	cases[142].name = "retrieve_onerule_cat_alt_inner_repeat_mixed_variation2_many";
	cases[142].test = retrieve_onerule_cat_alt_inner_repeat_mixed_variation2_many;

	cases[143].name = "retrieve_onerule_cat_alt_inner_repeat_mixed";
	cases[143].test = retrieve_onerule_cat_alt_inner_repeat_mixed;

	cases[144].name = "retrieve_onerule_nested_string";
	cases[144].test = retrieve_onerule_nested_string;

	cases[145].name = "retrieve_rule_dne_name";
	cases[145].test = retrieve_rule_dne_name;

	cases[146].name = "retrieve_rule_dne_index";
	cases[146].test = retrieve_rule_dne_index;

	cases[147].name = "retrieve_out_null";
	cases[147].test = retrieve_out_null;

	cases[148].name = "retrieve_onerule_repeated_empty";
	cases[148].test = retrieve_onerule_repeated_empty;

	cases[149].name = "retrieve_onerule_repeated_char";
	cases[149].test = retrieve_onerule_repeated_char;

	cases[150].name = "retrieve_onerule_repeated_char_range_i0";
	cases[150].test = retrieve_onerule_repeated_char_range_i0;

	cases[151].name = "retrieve_onerule_repeated_char_range_i1";
	cases[151].test = retrieve_onerule_repeated_char_range_i1;

	cases[152].name = "retrieve_onerule_repeated_char_range_i2";
	cases[152].test = retrieve_onerule_repeated_char_range_i2;

	cases[153].name = "retrieve_rule_concat_first";
	cases[153].test = retrieve_rule_concat_first;

	cases[154].name = "retrieve_rule_concat_middle";
	cases[154].test = retrieve_rule_concat_middle;

	cases[155].name = "retrieve_rule_concat_last";
	cases[155].test = retrieve_rule_concat_last;

	cases[156].name = "retrieve_rule_concat_dne_name";
	cases[156].test = retrieve_rule_concat_dne_name;

	cases[157].name = "retrieve_rule_concat_dne_index";
	cases[157].test = retrieve_rule_concat_dne_index;

	cases[158].name = "retrieve_rule_alt_varation1";
	cases[158].test = retrieve_rule_alt_varation1;

	cases[159].name = "retrieve_rule_alt_varation1_dne2";
	cases[159].test = retrieve_rule_alt_varation1_dne2;

	cases[160].name = "retrieve_rule_alt_varation1_dne3";
	cases[160].test = retrieve_rule_alt_varation1_dne3;

	cases[161].name = "retrieve_rule_alt_varation2";
	cases[161].test = retrieve_rule_alt_varation2;

	cases[162].name = "retrieve_rule_alt_varation2_dne1";
	cases[162].test = retrieve_rule_alt_varation2_dne1;

	cases[163].name = "retrieve_rule_alt_varation2_dne3";
	cases[163].test = retrieve_rule_alt_varation2_dne3;

	cases[164].name = "retrieve_rule_alt_varation3";
	cases[164].test = retrieve_rule_alt_varation3;

	cases[165].name = "retrieve_rule_alt_varation3_dne1";
	cases[165].test = retrieve_rule_alt_varation3_dne1;

	cases[166].name = "retrieve_rule_alt_varation3_dne2";
	cases[166].test = retrieve_rule_alt_varation3_dne2;

	cases[167].name = "retrieve_rule_altcat_varation1_first";
	cases[167].test = retrieve_rule_altcat_varation1_first;

	cases[168].name = "retrieve_rule_altcat_varation1_middle";
	cases[168].test = retrieve_rule_altcat_varation1_middle;

	cases[169].name = "retrieve_rule_altcat_varation2_first";
	cases[169].test = retrieve_rule_altcat_varation2_first;

	cases[170].name = "retrieve_rule_altcat_varation2_middle";
	cases[170].test = retrieve_rule_altcat_varation2_middle;

	cases[171].name = "retrieve_rule_altcat_varation3_first";
	cases[171].test = retrieve_rule_altcat_varation3_first;

	cases[172].name = "retrieve_rule_altcat_varation3_middle";
	cases[172].test = retrieve_rule_altcat_varation3_middle;

	cases[173].name = "retrieve_rule_catalt_first_variation1";
	cases[173].test = retrieve_rule_catalt_first_variation1;

	cases[174].name = "retrieve_rule_catalt_first_variation2";
	cases[174].test = retrieve_rule_catalt_first_variation2;

	cases[175].name = "retrieve_rule_catalt_alt_variation1";
	cases[175].test = retrieve_rule_catalt_alt_variation1;

	cases[176].name = "retrieve_rule_catalt_alt_variation1_dne2";
	cases[176].test = retrieve_rule_catalt_alt_variation1_dne2;

	cases[177].name = "retrieve_rule_catalt_alt_variation2_dne1";
	cases[177].test = retrieve_rule_catalt_alt_variation2_dne1;

	cases[178].name = "retrieve_rule_catalt_alt_variation2";
	cases[178].test = retrieve_rule_catalt_alt_variation2;

	cases[179].name = "retrieve_rule_catalt_middle_variation1";
	cases[179].test = retrieve_rule_catalt_middle_variation1;

	cases[180].name = "retrieve_rule_catalt_middle_variation2";
	cases[180].test = retrieve_rule_catalt_middle_variation2;

	cases[181].name = "retrieve_rule_cat_repeated_outer_first_i0";
	cases[181].test = retrieve_rule_cat_repeated_outer_first_i0;

	cases[182].name = "retrieve_rule_cat_repeated_outer_first_i1";
	cases[182].test = retrieve_rule_cat_repeated_outer_first_i1;

	cases[183].name = "retrieve_rule_cat_repeated_outer_first_i2";
	cases[183].test = retrieve_rule_cat_repeated_outer_first_i2;

	cases[184].name = "retrieve_rule_cat_repeated_outer_middle_i0";
	cases[184].test = retrieve_rule_cat_repeated_outer_middle_i0;

	cases[185].name = "retrieve_rule_cat_repeated_outer_middle_i1";
	cases[185].test = retrieve_rule_cat_repeated_outer_middle_i1;

	cases[186].name = "retrieve_rule_cat_repeated_outer_middle_i2";
	cases[186].test = retrieve_rule_cat_repeated_outer_middle_i2;

	cases[187].name = "retrieve_rule_cat_repeated_outer_last_i0";
	cases[187].test = retrieve_rule_cat_repeated_outer_last_i0;

	cases[188].name = "retrieve_rule_cat_repeated_outer_last_i1";
	cases[188].test = retrieve_rule_cat_repeated_outer_last_i1;

	cases[189].name = "retrieve_rule_cat_repeated_outer_last_i2";
	cases[189].test = retrieve_rule_cat_repeated_outer_last_i2;

	cases[190].name = "retrieve_rule_cat_repeated_outer_first_dne";
	cases[190].test = retrieve_rule_cat_repeated_outer_first_dne;

	cases[191].name = "retrieve_rule_cat_repeated_outer_middle_dne";
	cases[191].test = retrieve_rule_cat_repeated_outer_middle_dne;

	cases[192].name = "retrieve_rule_cat_repeated_outer_last_dne";
	cases[192].test = retrieve_rule_cat_repeated_outer_last_dne;

	cases[193].name = "retrieve_rule_cat_repeated_inner_empty_dne1";
	cases[193].test = retrieve_rule_cat_repeated_inner_empty_dne1;

	cases[194].name = "retrieve_rule_cat_repeated_inner_empty_dne2";
	cases[194].test = retrieve_rule_cat_repeated_inner_empty_dne2;

	cases[195].name = "retrieve_rule_cat_repeated_inner_empty_dne3";
	cases[195].test = retrieve_rule_cat_repeated_inner_empty_dne3;

	cases[196].name = "retrieve_rule_cat_repeated_inner_first_i0";
	cases[196].test = retrieve_rule_cat_repeated_inner_first_i0;

	cases[197].name = "retrieve_rule_cat_repeated_inner_first_i1";
	cases[197].test = retrieve_rule_cat_repeated_inner_first_i1;

	cases[198].name = "retrieve_rule_cat_repeated_inner_first_i2";
	cases[198].test = retrieve_rule_cat_repeated_inner_first_i2;

	cases[199].name = "retrieve_rule_cat_repeated_inner_first_dne_i3";
	cases[199].test = retrieve_rule_cat_repeated_inner_first_dne_i3;

	cases[200].name = "retrieve_rule_cat_repeated_inner_first_dne_middle";
	cases[200].test = retrieve_rule_cat_repeated_inner_first_dne_middle;

	cases[201].name = "retrieve_rule_cat_repeated_inner_first_dne_last";
	cases[201].test = retrieve_rule_cat_repeated_inner_first_dne_last;

	cases[202].name = "retrieve_rule_cat_repeated_inner_middle_i0";
	cases[202].test = retrieve_rule_cat_repeated_inner_middle_i0;

	cases[203].name = "retrieve_rule_cat_repeated_inner_middle_i1";
	cases[203].test = retrieve_rule_cat_repeated_inner_middle_i1;

	cases[204].name = "retrieve_rule_cat_repeated_inner_middle_i2";
	cases[204].test = retrieve_rule_cat_repeated_inner_middle_i2;

	cases[205].name = "retrieve_rule_cat_repeated_inner_middle_dne_i3";
	cases[205].test = retrieve_rule_cat_repeated_inner_middle_dne_i3;

	cases[206].name = "retrieve_rule_cat_repeated_inner_middle_dne_first";
	cases[206].test = retrieve_rule_cat_repeated_inner_middle_dne_first;

	cases[207].name = "retrieve_rule_cat_repeated_inner_middle_dne_last";
	cases[207].test = retrieve_rule_cat_repeated_inner_middle_dne_last;

	cases[208].name = "retrieve_rule_cat_repeated_inner_third_i0";
	cases[208].test = retrieve_rule_cat_repeated_inner_third_i0;

	cases[209].name = "retrieve_rule_cat_repeated_inner_third_i1";
	cases[209].test = retrieve_rule_cat_repeated_inner_third_i1;

	cases[210].name = "retrieve_rule_cat_repeated_inner_third_i2";
	cases[210].test = retrieve_rule_cat_repeated_inner_third_i2;

	cases[211].name = "retrieve_rule_cat_repeated_inner_third_dne_i3";
	cases[211].test = retrieve_rule_cat_repeated_inner_third_dne_i3;

	cases[212].name = "retrieve_rule_cat_repeated_inner_third_dne_first";
	cases[212].test = retrieve_rule_cat_repeated_inner_third_dne_first;

	cases[213].name = "retrieve_rule_cat_repeated_inner_third_dne_middle";
	cases[213].test = retrieve_rule_cat_repeated_inner_third_dne_middle;

	cases[214].name = "retrieve_rule_cat_repeated_inner_all_first_i0";
	cases[214].test = retrieve_rule_cat_repeated_inner_all_first_i0;

	cases[215].name = "retrieve_rule_cat_repeated_inner_all_first_i1";
	cases[215].test = retrieve_rule_cat_repeated_inner_all_first_i1;

	cases[216].name = "retrieve_rule_cat_repeated_inner_all_first_i2";
	cases[216].test = retrieve_rule_cat_repeated_inner_all_first_i2;

	cases[217].name = "retrieve_rule_cat_repeated_inner_all_first_dne_i3";
	cases[217].test = retrieve_rule_cat_repeated_inner_all_first_dne_i3;

	cases[218].name = "retrieve_rule_cat_repeated_inner_all_middle_i0";
	cases[218].test = retrieve_rule_cat_repeated_inner_all_middle_i0;

	cases[219].name = "retrieve_rule_cat_repeated_inner_all_middle_i1";
	cases[219].test = retrieve_rule_cat_repeated_inner_all_middle_i1;

	cases[220].name = "retrieve_rule_cat_repeated_inner_all_middle_i2";
	cases[220].test = retrieve_rule_cat_repeated_inner_all_middle_i2;

	cases[221].name = "retrieve_rule_cat_repeated_inner_all_middle_dne_i3";
	cases[221].test = retrieve_rule_cat_repeated_inner_all_middle_dne_i3;

	cases[222].name = "retrieve_rule_cat_repeated_inner_all_last_i0";
	cases[222].test = retrieve_rule_cat_repeated_inner_all_last_i0;

	cases[223].name = "retrieve_rule_cat_repeated_inner_all_last_i1";
	cases[223].test = retrieve_rule_cat_repeated_inner_all_last_i1;

	cases[224].name = "retrieve_rule_cat_repeated_inner_all_last_i2";
	cases[224].test = retrieve_rule_cat_repeated_inner_all_last_i2;

	cases[225].name = "retrieve_rule_cat_repeated_inner_all_last_dne_i3";
	cases[225].test = retrieve_rule_cat_repeated_inner_all_last_dne_i3;

	cases[226].name = "retrieve_rule_alt_repeated_outer_empty_dne1";
	cases[226].test = retrieve_rule_alt_repeated_outer_empty_dne1;

	cases[227].name = "retrieve_rule_alt_repeated_outer_empty_dne2";
	cases[227].test = retrieve_rule_alt_repeated_outer_empty_dne2;

	cases[228].name = "retrieve_rule_alt_repeated_outer_empty_dne3";
	cases[228].test = retrieve_rule_alt_repeated_outer_empty_dne3;

	cases[229].name = "retrieve_rule_alt_repeated_outer_variation1_i0";
	cases[229].test = retrieve_rule_alt_repeated_outer_variation1_i0;

	cases[230].name = "retrieve_rule_alt_repeated_outer_variation1_i1";
	cases[230].test = retrieve_rule_alt_repeated_outer_variation1_i1;

	cases[231].name = "retrieve_rule_alt_repeated_outer_variation1_i2";
	cases[231].test = retrieve_rule_alt_repeated_outer_variation1_i2;

	cases[232].name = "retrieve_rule_alt_repeated_outer_variation1_dne_i3";
	cases[232].test = retrieve_rule_alt_repeated_outer_variation1_dne_i3;

	cases[233].name = "retrieve_rule_alt_repeated_outer_variation1_dne_var2";
	cases[233].test = retrieve_rule_alt_repeated_outer_variation1_dne_var2;

	cases[234].name = "retrieve_rule_alt_repeated_outer_variation1_dne_var3";
	cases[234].test = retrieve_rule_alt_repeated_outer_variation1_dne_var3;

	cases[235].name = "retrieve_rule_alt_repeated_outer_variation2_i0";
	cases[235].test = retrieve_rule_alt_repeated_outer_variation2_i0;

	cases[236].name = "retrieve_rule_alt_repeated_outer_variation2_i1";
	cases[236].test = retrieve_rule_alt_repeated_outer_variation2_i1;

	cases[237].name = "retrieve_rule_alt_repeated_outer_variation2_i2";
	cases[237].test = retrieve_rule_alt_repeated_outer_variation2_i2;

	cases[238].name = "retrieve_rule_alt_repeated_outer_variation2_dne_i3";
	cases[238].test = retrieve_rule_alt_repeated_outer_variation2_dne_i3;

	cases[239].name = "retrieve_rule_alt_repeated_outer_variation2_dne_var1";
	cases[239].test = retrieve_rule_alt_repeated_outer_variation2_dne_var1;

	cases[240].name = "retrieve_rule_alt_repeated_outer_variation2_dne_var3";
	cases[240].test = retrieve_rule_alt_repeated_outer_variation2_dne_var3;

	cases[241].name = "retrieve_rule_alt_repeated_outer_variation3_i0";
	cases[241].test = retrieve_rule_alt_repeated_outer_variation3_i0;

	cases[242].name = "retrieve_rule_alt_repeated_outer_variation3_i1";
	cases[242].test = retrieve_rule_alt_repeated_outer_variation3_i1;

	cases[243].name = "retrieve_rule_alt_repeated_outer_variation3_i2";
	cases[243].test = retrieve_rule_alt_repeated_outer_variation3_i2;

	cases[244].name = "retrieve_rule_alt_repeated_outer_variation3_dne_i3";
	cases[244].test = retrieve_rule_alt_repeated_outer_variation3_dne_i3;

	cases[245].name = "retrieve_rule_alt_repeated_outer_variation3_dne_var1";
	cases[245].test = retrieve_rule_alt_repeated_outer_variation3_dne_var1;

	cases[246].name = "retrieve_rule_alt_repeated_outer_variation3_dne_var2";
	cases[246].test = retrieve_rule_alt_repeated_outer_variation3_dne_var2;

	cases[247].name = "retrieve_rule_alt_repeated_outer_all_variation1_i0";
	cases[247].test = retrieve_rule_alt_repeated_outer_all_variation1_i0;

	cases[248].name = "retrieve_rule_alt_repeated_outer_all_variation1_i1";
	cases[248].test = retrieve_rule_alt_repeated_outer_all_variation1_i1;

	cases[249].name = "retrieve_rule_alt_repeated_outer_all_variation1_i2";
	cases[249].test = retrieve_rule_alt_repeated_outer_all_variation1_i2;

	cases[250].name = "retrieve_rule_alt_repeated_outer_all_variation1_i3";
	cases[250].test = retrieve_rule_alt_repeated_outer_all_variation1_i3;

	cases[251].name = "retrieve_rule_alt_repeated_outer_all_variation2_i0";
	cases[251].test = retrieve_rule_alt_repeated_outer_all_variation2_i0;

	cases[252].name = "retrieve_rule_alt_repeated_outer_all_variation2_i1";
	cases[252].test = retrieve_rule_alt_repeated_outer_all_variation2_i1;

	cases[253].name = "retrieve_rule_alt_repeated_outer_all_variation2_i2";
	cases[253].test = retrieve_rule_alt_repeated_outer_all_variation2_i2;

	cases[254].name = "retrieve_rule_alt_repeated_outer_all_variation3_i0";
	cases[254].test = retrieve_rule_alt_repeated_outer_all_variation3_i0;

	cases[255].name = "retrieve_rule_alt_repeated_outer_all_variation3_i1";
	cases[255].test = retrieve_rule_alt_repeated_outer_all_variation3_i1;

	cases[256].name = "retrieve_rule_alt_repeated_outer_all_variation3_i2";
	cases[256].test = retrieve_rule_alt_repeated_outer_all_variation3_i2;

	cases[257].name = "retrieve_rule_alt_repeated_outer_all_dne_variation1_i3";
	cases[257].test = retrieve_rule_alt_repeated_outer_all_dne_variation1_i3;

	cases[258].name = "retrieve_rule_alt_repeated_outer_all_dne_variation2_i3";
	cases[258].test = retrieve_rule_alt_repeated_outer_all_dne_variation2_i3;

	cases[259].name = "retrieve_rule_alt_repeated_outer_all_dne_variation3_i3";
	cases[259].test = retrieve_rule_alt_repeated_outer_all_dne_variation3_i3;

	cases[260].name = "retrieve_rule_altcat_repeated_outer_variation1_first_i0";
	cases[260].test = retrieve_rule_altcat_repeated_outer_variation1_first_i0;

	cases[261].name = "retrieve_rule_altcat_repeated_outer_variation1_first_i1";
	cases[261].test = retrieve_rule_altcat_repeated_outer_variation1_first_i1;

	cases[262].name = "retrieve_rule_altcat_repeated_outer_variation1_first_i2";
	cases[262].test = retrieve_rule_altcat_repeated_outer_variation1_first_i2;

	cases[263].name = "retrieve_rule_altcat_repeated_outer_variation1_second_i0";
	cases[263].test = retrieve_rule_altcat_repeated_outer_variation1_second_i0;

	cases[264].name = "retrieve_rule_altcat_repeated_outer_variation1_second_i1";
	cases[264].test = retrieve_rule_altcat_repeated_outer_variation1_second_i1;

	cases[265].name = "retrieve_rule_altcat_repeated_outer_variation1_second_i2";
	cases[265].test = retrieve_rule_altcat_repeated_outer_variation1_second_i2;

	cases[266].name = "retrieve_rule_altcat_repeated_outer_variation1_dne_first_i3";
	cases[266].test = retrieve_rule_altcat_repeated_outer_variation1_dne_first_i3;

	cases[267].name = "retrieve_rule_altcat_repeated_outer_variation1_dne_second_i3";
	cases[267].test = retrieve_rule_altcat_repeated_outer_variation1_dne_second_i3;

	cases[268].name = "retrieve_rule_altcat_repeated_outer_variation1_dne3";
	cases[268].test = retrieve_rule_altcat_repeated_outer_variation1_dne3;

	cases[269].name = "retrieve_rule_altcat_repeated_outer_variation1_dne4";
	cases[269].test = retrieve_rule_altcat_repeated_outer_variation1_dne4;

	cases[270].name = "retrieve_rule_altcat_repeated_outer_variation1_dne5";
	cases[270].test = retrieve_rule_altcat_repeated_outer_variation1_dne5;

	cases[271].name = "retrieve_rule_altcat_repeated_outer_variation1_dne6";
	cases[271].test = retrieve_rule_altcat_repeated_outer_variation1_dne6;

	cases[272].name = "retrieve_rule_altcat_repeated_outer_variation2_first_i0";
	cases[272].test = retrieve_rule_altcat_repeated_outer_variation2_first_i0;

	cases[273].name = "retrieve_rule_altcat_repeated_outer_variation2_first_i1";
	cases[273].test = retrieve_rule_altcat_repeated_outer_variation2_first_i1;

	cases[274].name = "retrieve_rule_altcat_repeated_outer_variation2_first_i2";
	cases[274].test = retrieve_rule_altcat_repeated_outer_variation2_first_i2;

	cases[275].name = "retrieve_rule_altcat_repeated_outer_variation2_second_i0";
	cases[275].test = retrieve_rule_altcat_repeated_outer_variation2_second_i0;

	cases[276].name = "retrieve_rule_altcat_repeated_outer_variation2_second_i1";
	cases[276].test = retrieve_rule_altcat_repeated_outer_variation2_second_i1;

	cases[277].name = "retrieve_rule_altcat_repeated_outer_variation2_second_i2";
	cases[277].test = retrieve_rule_altcat_repeated_outer_variation2_second_i2;

	cases[278].name = "retrieve_rule_altcat_repeated_outer_variation2_dne_first_i3";
	cases[278].test = retrieve_rule_altcat_repeated_outer_variation2_dne_first_i3;

	cases[279].name = "retrieve_rule_altcat_repeated_outer_variation2_dne_second_i3";
	cases[279].test = retrieve_rule_altcat_repeated_outer_variation2_dne_second_i3;

	cases[280].name = "retrieve_rule_altcat_repeated_outer_variation2_dne1";
	cases[280].test = retrieve_rule_altcat_repeated_outer_variation2_dne1;

	cases[281].name = "retrieve_rule_altcat_repeated_outer_variation2_dne2";
	cases[281].test = retrieve_rule_altcat_repeated_outer_variation2_dne2;

	cases[282].name = "retrieve_rule_altcat_repeated_outer_variation2_dne5";
	cases[282].test = retrieve_rule_altcat_repeated_outer_variation2_dne5;

	cases[283].name = "retrieve_rule_altcat_repeated_outer_variation2_dne6";
	cases[283].test = retrieve_rule_altcat_repeated_outer_variation2_dne6;

	cases[284].name = "retrieve_rule_altcat_repeated_outer_variation3_first_i0";
	cases[284].test = retrieve_rule_altcat_repeated_outer_variation3_first_i0;

	cases[285].name = "retrieve_rule_altcat_repeated_outer_variation3_first_i1";
	cases[285].test = retrieve_rule_altcat_repeated_outer_variation3_first_i1;

	cases[286].name = "retrieve_rule_altcat_repeated_outer_variation3_first_i2";
	cases[286].test = retrieve_rule_altcat_repeated_outer_variation3_first_i2;

	cases[287].name = "retrieve_rule_altcat_repeated_outer_variation3_second_i0";
	cases[287].test = retrieve_rule_altcat_repeated_outer_variation3_second_i0;

	cases[288].name = "retrieve_rule_altcat_repeated_outer_variation3_second_i1";
	cases[288].test = retrieve_rule_altcat_repeated_outer_variation3_second_i1;

	cases[289].name = "retrieve_rule_altcat_repeated_outer_variation3_second_i2";
	cases[289].test = retrieve_rule_altcat_repeated_outer_variation3_second_i2;

	cases[290].name = "retrieve_rule_altcat_repeated_outer_variation3_dne_first_i3";
	cases[290].test = retrieve_rule_altcat_repeated_outer_variation3_dne_first_i3;

	cases[291].name = "retrieve_rule_altcat_repeated_outer_variation3_dne_second_i3";
	cases[291].test = retrieve_rule_altcat_repeated_outer_variation3_dne_second_i3;

	cases[292].name = "retrieve_rule_altcat_repeated_outer_variation3_dne1";
	cases[292].test = retrieve_rule_altcat_repeated_outer_variation3_dne1;

	cases[293].name = "retrieve_rule_altcat_repeated_outer_variation3_dne2";
	cases[293].test = retrieve_rule_altcat_repeated_outer_variation3_dne2;

	cases[294].name = "retrieve_rule_altcat_repeated_outer_variation3_dne3";
	cases[294].test = retrieve_rule_altcat_repeated_outer_variation3_dne3;

	cases[295].name = "retrieve_rule_altcat_repeated_outer_variation3_dne4";
	cases[295].test = retrieve_rule_altcat_repeated_outer_variation3_dne4;

	cases[296].name = "retrieve_rule_altcat_repeated_outer_all_variation1_first_i0";
	cases[296].test = retrieve_rule_altcat_repeated_outer_all_variation1_first_i0;

	cases[297].name = "retrieve_rule_altcat_repeated_outer_all_variation1_first_i1";
	cases[297].test = retrieve_rule_altcat_repeated_outer_all_variation1_first_i1;

	cases[298].name = "retrieve_rule_altcat_repeated_outer_all_variation1_first_i2";
	cases[298].test = retrieve_rule_altcat_repeated_outer_all_variation1_first_i2;

	cases[299].name = "retrieve_rule_altcat_repeated_outer_all_variation1_second_i0";
	cases[299].test = retrieve_rule_altcat_repeated_outer_all_variation1_second_i0;

	cases[300].name = "retrieve_rule_altcat_repeated_outer_all_variation1_second_i1";
	cases[300].test = retrieve_rule_altcat_repeated_outer_all_variation1_second_i1;

	cases[301].name = "retrieve_rule_altcat_repeated_outer_all_variation1_second_i2";
	cases[301].test = retrieve_rule_altcat_repeated_outer_all_variation1_second_i2;

	cases[302].name = "retrieve_rule_altcat_repeated_outer_all_variation2_first_i0";
	cases[302].test = retrieve_rule_altcat_repeated_outer_all_variation2_first_i0;

	cases[303].name = "retrieve_rule_altcat_repeated_outer_all_variation2_first_i1";
	cases[303].test = retrieve_rule_altcat_repeated_outer_all_variation2_first_i1;

	cases[304].name = "retrieve_rule_altcat_repeated_outer_all_variation2_first_i2";
	cases[304].test = retrieve_rule_altcat_repeated_outer_all_variation2_first_i2;

	cases[305].name = "retrieve_rule_altcat_repeated_outer_all_variation2_second_i0";
	cases[305].test = retrieve_rule_altcat_repeated_outer_all_variation2_second_i0;

	cases[306].name = "retrieve_rule_altcat_repeated_outer_all_variation2_second_i1";
	cases[306].test = retrieve_rule_altcat_repeated_outer_all_variation2_second_i1;

	cases[307].name = "retrieve_rule_altcat_repeated_outer_all_variation2_second_i2";
	cases[307].test = retrieve_rule_altcat_repeated_outer_all_variation2_second_i2;

	cases[308].name = "retrieve_rule_altcat_repeated_outer_all_variation3_first_i0";
	cases[308].test = retrieve_rule_altcat_repeated_outer_all_variation3_first_i0;

	cases[309].name = "retrieve_rule_altcat_repeated_outer_all_variation3_first_i1";
	cases[309].test = retrieve_rule_altcat_repeated_outer_all_variation3_first_i1;

	cases[310].name = "retrieve_rule_altcat_repeated_outer_all_variation3_first_i2";
	cases[310].test = retrieve_rule_altcat_repeated_outer_all_variation3_first_i2;

	cases[311].name = "retrieve_rule_altcat_repeated_outer_all_variation3_second_i0";
	cases[311].test = retrieve_rule_altcat_repeated_outer_all_variation3_second_i0;

	cases[312].name = "retrieve_rule_altcat_repeated_outer_all_variation3_second_i1";
	cases[312].test = retrieve_rule_altcat_repeated_outer_all_variation3_second_i1;

	cases[313].name = "retrieve_rule_altcat_repeated_outer_all_variation3_second_i2";
	cases[313].test = retrieve_rule_altcat_repeated_outer_all_variation3_second_i2;

	cases[314].name = "retrieve_rule_altcat_repeated_outer_all_dne_variation1_first_i3";
	cases[314].test = retrieve_rule_altcat_repeated_outer_all_dne_variation1_first_i3;

	cases[315].name = "retrieve_rule_altcat_repeated_outer_all_dne_variation1_second_i3";
	cases[315].test = retrieve_rule_altcat_repeated_outer_all_dne_variation1_second_i3;

	cases[316].name = "retrieve_rule_altcat_repeated_outer_all_dne_variation2_first_i3";
	cases[316].test = retrieve_rule_altcat_repeated_outer_all_dne_variation2_first_i3;

	cases[317].name = "retrieve_rule_altcat_repeated_outer_all_dne_variation2_second_i3";
	cases[317].test = retrieve_rule_altcat_repeated_outer_all_dne_variation2_second_i3;

	cases[318].name = "retrieve_rule_altcat_repeated_outer_all_dne_variation3_first_i3";
	cases[318].test = retrieve_rule_altcat_repeated_outer_all_dne_variation3_first_i3;

	cases[319].name = "retrieve_rule_altcat_repeated_outer_all_dne_variation3_second_i3";
	cases[319].test = retrieve_rule_altcat_repeated_outer_all_dne_variation3_second_i3;

	cases[320].name = "retrieve_rule_altcat_repeated_inner_variation1_first_i0";
	cases[320].test = retrieve_rule_altcat_repeated_inner_variation1_first_i0;

	cases[321].name = "retrieve_rule_altcat_repeated_inner_variation1_first_i1";
	cases[321].test = retrieve_rule_altcat_repeated_inner_variation1_first_i1;

	cases[322].name = "retrieve_rule_altcat_repeated_inner_variation1_first_i2";
	cases[322].test = retrieve_rule_altcat_repeated_inner_variation1_first_i2;

	cases[323].name = "retrieve_rule_altcat_repeated_inner_variation1_second_i0";
	cases[323].test = retrieve_rule_altcat_repeated_inner_variation1_second_i0;

	cases[324].name = "retrieve_rule_altcat_repeated_inner_variation1_second_i1";
	cases[324].test = retrieve_rule_altcat_repeated_inner_variation1_second_i1;

	cases[325].name = "retrieve_rule_altcat_repeated_inner_variation1_second_i2";
	cases[325].test = retrieve_rule_altcat_repeated_inner_variation1_second_i2;

	cases[326].name = "retrieve_rule_altcat_repeated_inner_variation1_dne_first_i3";
	cases[326].test = retrieve_rule_altcat_repeated_inner_variation1_dne_first_i3;

	cases[327].name = "retrieve_rule_altcat_repeated_inner_variation1_dne_second_i3";
	cases[327].test = retrieve_rule_altcat_repeated_inner_variation1_dne_second_i3;

	cases[328].name = "retrieve_rule_altcat_repeated_inner_variation1_dne3";
	cases[328].test = retrieve_rule_altcat_repeated_inner_variation1_dne3;

	cases[329].name = "retrieve_rule_altcat_repeated_inner_variation1_dne4";
	cases[329].test = retrieve_rule_altcat_repeated_inner_variation1_dne4;

	cases[330].name = "retrieve_rule_altcat_repeated_inner_variation1_dne5";
	cases[330].test = retrieve_rule_altcat_repeated_inner_variation1_dne5;

	cases[331].name = "retrieve_rule_altcat_repeated_inner_variation1_dne6";
	cases[331].test = retrieve_rule_altcat_repeated_inner_variation1_dne6;

	cases[332].name = "retrieve_rule_altcat_repeated_inner_variation2_first_i0";
	cases[332].test = retrieve_rule_altcat_repeated_inner_variation2_first_i0;

	cases[333].name = "retrieve_rule_altcat_repeated_inner_variation2_first_i1";
	cases[333].test = retrieve_rule_altcat_repeated_inner_variation2_first_i1;

	cases[334].name = "retrieve_rule_altcat_repeated_inner_variation2_first_i2";
	cases[334].test = retrieve_rule_altcat_repeated_inner_variation2_first_i2;

	cases[335].name = "retrieve_rule_altcat_repeated_inner_variation2_second_i0";
	cases[335].test = retrieve_rule_altcat_repeated_inner_variation2_second_i0;

	cases[336].name = "retrieve_rule_altcat_repeated_inner_variation2_second_i1";
	cases[336].test = retrieve_rule_altcat_repeated_inner_variation2_second_i1;

	cases[337].name = "retrieve_rule_altcat_repeated_inner_variation2_second_i2";
	cases[337].test = retrieve_rule_altcat_repeated_inner_variation2_second_i2;

	cases[338].name = "retrieve_rule_altcat_repeated_inner_variation2_dne_first_i3";
	cases[338].test = retrieve_rule_altcat_repeated_inner_variation2_dne_first_i3;

	cases[339].name = "retrieve_rule_altcat_repeated_inner_variation2_dne_second_i3";
	cases[339].test = retrieve_rule_altcat_repeated_inner_variation2_dne_second_i3;

	cases[340].name = "retrieve_rule_altcat_repeated_inner_variation2_dne1";
	cases[340].test = retrieve_rule_altcat_repeated_inner_variation2_dne1;

	cases[341].name = "retrieve_rule_altcat_repeated_inner_variation2_dne2";
	cases[341].test = retrieve_rule_altcat_repeated_inner_variation2_dne2;

	cases[342].name = "retrieve_rule_altcat_repeated_inner_variation2_dne5";
	cases[342].test = retrieve_rule_altcat_repeated_inner_variation2_dne5;

	cases[343].name = "retrieve_rule_altcat_repeated_inner_variation2_dne6";
	cases[343].test = retrieve_rule_altcat_repeated_inner_variation2_dne6;

	cases[344].name = "retrieve_rule_altcat_repeated_inner_variation3_first_i0";
	cases[344].test = retrieve_rule_altcat_repeated_inner_variation3_first_i0;

	cases[345].name = "retrieve_rule_altcat_repeated_inner_variation3_first_i1";
	cases[345].test = retrieve_rule_altcat_repeated_inner_variation3_first_i1;

	cases[346].name = "retrieve_rule_altcat_repeated_inner_variation3_first_i2";
	cases[346].test = retrieve_rule_altcat_repeated_inner_variation3_first_i2;

	cases[347].name = "retrieve_rule_altcat_repeated_inner_variation3_second_i0";
	cases[347].test = retrieve_rule_altcat_repeated_inner_variation3_second_i0;

	cases[348].name = "retrieve_rule_altcat_repeated_inner_variation3_second_i1";
	cases[348].test = retrieve_rule_altcat_repeated_inner_variation3_second_i1;

	cases[349].name = "retrieve_rule_altcat_repeated_inner_variation3_second_i2";
	cases[349].test = retrieve_rule_altcat_repeated_inner_variation3_second_i2;

	cases[350].name = "retrieve_rule_altcat_repeated_inner_variation3_dne_first_i3";
	cases[350].test = retrieve_rule_altcat_repeated_inner_variation3_dne_first_i3;

	cases[351].name = "retrieve_rule_altcat_repeated_inner_variation3_dne_second_i3";
	cases[351].test = retrieve_rule_altcat_repeated_inner_variation3_dne_second_i3;

	cases[352].name = "retrieve_rule_altcat_repeated_inner_variation3_dne1";
	cases[352].test = retrieve_rule_altcat_repeated_inner_variation3_dne1;

	cases[353].name = "retrieve_rule_altcat_repeated_inner_variation3_dne2";
	cases[353].test = retrieve_rule_altcat_repeated_inner_variation3_dne2;

	cases[354].name = "retrieve_rule_altcat_repeated_inner_variation3_dne3";
	cases[354].test = retrieve_rule_altcat_repeated_inner_variation3_dne3;

	cases[355].name = "retrieve_rule_altcat_repeated_inner_variation3_dne4";
	cases[355].test = retrieve_rule_altcat_repeated_inner_variation3_dne4;

	cases[356].name = "retrieve_rule_catalt_repeated_outer_v1_first_i0";
	cases[356].test = retrieve_rule_catalt_repeated_outer_v1_first_i0;

	cases[357].name = "retrieve_rule_catalt_repeated_outer_v1_first_i1";
	cases[357].test = retrieve_rule_catalt_repeated_outer_v1_first_i1;

	cases[358].name = "retrieve_rule_catalt_repeated_outer_v1_first_i2_dne";
	cases[358].test = retrieve_rule_catalt_repeated_outer_v1_first_i2_dne;

	cases[359].name = "retrieve_rule_catalt_repeated_outer_v1_second_variation1_i0";
	cases[359].test = retrieve_rule_catalt_repeated_outer_v1_second_variation1_i0;

	cases[360].name = "retrieve_rule_catalt_repeated_outer_v1_second_variation1_i1";
	cases[360].test = retrieve_rule_catalt_repeated_outer_v1_second_variation1_i1;

	cases[361].name = "retrieve_rule_catalt_repeated_outer_v1_second_variation1_i2_dne";
	cases[361].test = retrieve_rule_catalt_repeated_outer_v1_second_variation1_i2_dne;

	cases[362].name = "retrieve_rule_catalt_repeated_outer_v1_second_variation2_dne";
	cases[362].test = retrieve_rule_catalt_repeated_outer_v1_second_variation2_dne;

	cases[363].name = "retrieve_rule_catalt_repeated_outer_v1_last_i0";
	cases[363].test = retrieve_rule_catalt_repeated_outer_v1_last_i0;

	cases[364].name = "retrieve_rule_catalt_repeated_outer_v1_last_i1";
	cases[364].test = retrieve_rule_catalt_repeated_outer_v1_last_i1;

	cases[365].name = "retrieve_rule_catalt_repeated_outer_v1_last_i2_dne";
	cases[365].test = retrieve_rule_catalt_repeated_outer_v1_last_i2_dne;

	cases[366].name = "retrieve_rule_catalt_repeated_outer_v2_first_i0";
	cases[366].test = retrieve_rule_catalt_repeated_outer_v2_first_i0;

	cases[367].name = "retrieve_rule_catalt_repeated_outer_v2_first_i1";
	cases[367].test = retrieve_rule_catalt_repeated_outer_v2_first_i1;

	cases[368].name = "retrieve_rule_catalt_repeated_outer_v2_second_variation1_dne";
	cases[368].test = retrieve_rule_catalt_repeated_outer_v2_second_variation1_dne;

	cases[369].name = "retrieve_rule_catalt_repeated_outer_v2_second_variation2_i0";
	cases[369].test = retrieve_rule_catalt_repeated_outer_v2_second_variation2_i0;

	cases[370].name = "retrieve_rule_catalt_repeated_outer_v2_second_variation2_i1";
	cases[370].test = retrieve_rule_catalt_repeated_outer_v2_second_variation2_i1;

	cases[371].name = "retrieve_rule_catalt_repeated_outer_v2_second_variation2_i2_dne";
	cases[371].test = retrieve_rule_catalt_repeated_outer_v2_second_variation2_i2_dne;

	cases[372].name = "retrieve_rule_catalt_repeated_outer_v2_last_i0";
	cases[372].test = retrieve_rule_catalt_repeated_outer_v2_last_i0;

	cases[373].name = "retrieve_rule_catalt_repeated_outer_v2_last_i1";
	cases[373].test = retrieve_rule_catalt_repeated_outer_v2_last_i1;

	cases[374].name = "retrieve_rule_catalt_repeated_outer_v2_last_i2_dne";
	cases[374].test = retrieve_rule_catalt_repeated_outer_v2_last_i2_dne;

	cases[375].name = "retrieve_rule_catalt_repeated_outer_mixed_first_i0";
	cases[375].test = retrieve_rule_catalt_repeated_outer_mixed_first_i0;

	cases[376].name = "retrieve_rule_catalt_repeated_outer_mixed_first_i1";
	cases[376].test = retrieve_rule_catalt_repeated_outer_mixed_first_i1;

	cases[377].name = "retrieve_rule_catalt_repeated_outer_mixed_first_i2_dne";
	cases[377].test = retrieve_rule_catalt_repeated_outer_mixed_first_i2_dne;

	cases[378].name = "retrieve_rule_catalt_repeated_outer_mixed_second_variation1_i0";
	cases[378].test = retrieve_rule_catalt_repeated_outer_mixed_second_variation1_i0;

	cases[379].name = "retrieve_rule_catalt_repeated_outer_mixed_second_variation1_i1_dne";
	cases[379].test = retrieve_rule_catalt_repeated_outer_mixed_second_variation1_i1_dne;

	cases[380].name = "retrieve_rule_catalt_repeated_outer_mixed_second_variation2_i0";
	cases[380].test = retrieve_rule_catalt_repeated_outer_mixed_second_variation2_i0;

	cases[381].name = "retrieve_rule_catalt_repeated_outer_mixed_second_variation2_i1_dne";
	cases[381].test = retrieve_rule_catalt_repeated_outer_mixed_second_variation2_i1_dne;

	cases[382].name = "retrieve_rule_catalt_repeated_outer_mixed_last_i0";
	cases[382].test = retrieve_rule_catalt_repeated_outer_mixed_last_i0;

	cases[383].name = "retrieve_rule_catalt_repeated_outer_mixed_last_i1";
	cases[383].test = retrieve_rule_catalt_repeated_outer_mixed_last_i1;

	cases[384].name = "retrieve_rule_catalt_repeated_outer_mixed_last_i2_dne";
	cases[384].test = retrieve_rule_catalt_repeated_outer_mixed_last_i2_dne;

	cases[385].name = "retrieve_rule_catalt_repeated_inner_v1_first_i0";
	cases[385].test = retrieve_rule_catalt_repeated_inner_v1_first_i0;

	cases[386].name = "retrieve_rule_catalt_repeated_inner_v1_first_i1";
	cases[386].test = retrieve_rule_catalt_repeated_inner_v1_first_i1;

	cases[387].name = "retrieve_rule_catalt_repeated_inner_v1_first_i2_dne";
	cases[387].test = retrieve_rule_catalt_repeated_inner_v1_first_i2_dne;

	cases[388].name = "retrieve_rule_catalt_repeated_inner_v1_second_variation1_i0";
	cases[388].test = retrieve_rule_catalt_repeated_inner_v1_second_variation1_i0;

	cases[389].name = "retrieve_rule_catalt_repeated_inner_v1_second_variation1_i1";
	cases[389].test = retrieve_rule_catalt_repeated_inner_v1_second_variation1_i1;

	cases[390].name = "retrieve_rule_catalt_repeated_inner_v1_second_variation1_i2_dne";
	cases[390].test = retrieve_rule_catalt_repeated_inner_v1_second_variation1_i2_dne;

	cases[391].name = "retrieve_rule_catalt_repeated_inner_v1_second_variation2_dne";
	cases[391].test = retrieve_rule_catalt_repeated_inner_v1_second_variation2_dne;

	cases[392].name = "retrieve_rule_catalt_repeated_inner_v1_last_i0";
	cases[392].test = retrieve_rule_catalt_repeated_inner_v1_last_i0;

	cases[393].name = "retrieve_rule_catalt_repeated_inner_v1_last_i1";
	cases[393].test = retrieve_rule_catalt_repeated_inner_v1_last_i1;

	cases[394].name = "retrieve_rule_catalt_repeated_inner_v1_last_i2_dne";
	cases[394].test = retrieve_rule_catalt_repeated_inner_v1_last_i2_dne;

	cases[395].name = "retrieve_rule_catalt_repeated_inner_v2_first_i0";
	cases[395].test = retrieve_rule_catalt_repeated_inner_v2_first_i0;

	cases[396].name = "retrieve_rule_catalt_repeated_inner_v2_first_i1";
	cases[396].test = retrieve_rule_catalt_repeated_inner_v2_first_i1;

	cases[397].name = "retrieve_rule_catalt_repeated_inner_v2_first_i2_dne";
	cases[397].test = retrieve_rule_catalt_repeated_inner_v2_first_i2_dne;

	cases[398].name = "retrieve_rule_catalt_repeated_inner_v2_second_variation1_dne";
	cases[398].test = retrieve_rule_catalt_repeated_inner_v2_second_variation1_dne;

	cases[399].name = "retrieve_rule_catalt_repeated_inner_v2_second_variation2_i0";
	cases[399].test = retrieve_rule_catalt_repeated_inner_v2_second_variation2_i0;

	cases[400].name = "retrieve_rule_catalt_repeated_inner_v2_second_variation2_i1";
	cases[400].test = retrieve_rule_catalt_repeated_inner_v2_second_variation2_i1;

	cases[401].name = "retrieve_rule_catalt_repeated_inner_v2_second_variation2_i2_dne";
	cases[401].test = retrieve_rule_catalt_repeated_inner_v2_second_variation2_i2_dne;

	cases[402].name = "retrieve_rule_catalt_repeated_inner_v2_last_i0";
	cases[402].test = retrieve_rule_catalt_repeated_inner_v2_last_i0;

	cases[403].name = "retrieve_rule_catalt_repeated_inner_v2_last_i1";
	cases[403].test = retrieve_rule_catalt_repeated_inner_v2_last_i1;

	cases[404].name = "retrieve_rule_catalt_repeated_inner_v2_last_i2_dne";
	cases[404].test = retrieve_rule_catalt_repeated_inner_v2_last_i2_dne;

	cases[405].name = "retrieve_rule_catalt_repeated_inner_mixed_first_i0";
	cases[405].test = retrieve_rule_catalt_repeated_inner_mixed_first_i0;

	cases[406].name = "retrieve_rule_catalt_repeated_inner_mixed_first_i1";
	cases[406].test = retrieve_rule_catalt_repeated_inner_mixed_first_i1;

	cases[407].name = "retrieve_rule_catalt_repeated_inner_mixed_first_i2_dne";
	cases[407].test = retrieve_rule_catalt_repeated_inner_mixed_first_i2_dne;

	cases[408].name = "retrieve_rule_catalt_repeated_inner_mixed_second_variation1_i0";
	cases[408].test = retrieve_rule_catalt_repeated_inner_mixed_second_variation1_i0;

	cases[409].name = "retrieve_rule_catalt_repeated_inner_mixed_second_variation1_i1_dne";
	cases[409].test = retrieve_rule_catalt_repeated_inner_mixed_second_variation1_i1_dne;

	cases[410].name = "retrieve_rule_catalt_repeated_inner_mixed_second_variation2_i0";
	cases[410].test = retrieve_rule_catalt_repeated_inner_mixed_second_variation2_i0;

	cases[411].name = "retrieve_rule_catalt_repeated_inner_mixed_second_variation2_i1_dne";
	cases[411].test = retrieve_rule_catalt_repeated_inner_mixed_second_variation2_i1_dne;

	cases[412].name = "retrieve_rule_catalt_repeated_inner_mixed_last_i0";
	cases[412].test = retrieve_rule_catalt_repeated_inner_mixed_last_i0;

	cases[413].name = "retrieve_rule_catalt_repeated_inner_mixed_last_i1";
	cases[413].test = retrieve_rule_catalt_repeated_inner_mixed_last_i1;

	cases[414].name = "retrieve_rule_catalt_repeated_inner_mixed_last_i2_dne";
	cases[414].test = retrieve_rule_catalt_repeated_inner_mixed_last_i2_dne;

	cases[415].name = "retrieve_abnf_hexval_i0";
	cases[415].test = retrieve_abnf_hexval_i0;

	cases[416].name = "retrieve_abnf_hexval_i1";
	cases[416].test = retrieve_abnf_hexval_i1;

	cases[417].name = "retrieve_abnf_numval";
	cases[417].test = retrieve_abnf_numval;

	cases[418].name = "retrieve_abnf_numval_dec_dne";
	cases[418].test = retrieve_abnf_numval_dec_dne;

	cases[419].name = "retrieve_abnf_numval_bin_dne";
	cases[419].test = retrieve_abnf_numval_bin_dne;

	cases[420].name = "retrieve_abnf_charval_i0";
	cases[420].test = retrieve_abnf_charval_i0;

	cases[421].name = "retrieve_abnf_charval_i1";
	cases[421].test = retrieve_abnf_charval_i1;

	cases[422].name = "retrieve_abnf_option";
	cases[422].test = retrieve_abnf_option;

	cases[423].name = "retrieve_abnf_repeat_i0";
	cases[423].test = retrieve_abnf_repeat_i0;

	cases[424].name = "retrieve_abnf_repeat_i1";
	cases[424].test = retrieve_abnf_repeat_i1;

	cases[425].name = "retrieve_abnf_repeat_i2";
	cases[425].test = retrieve_abnf_repeat_i2;

	cases[426].name = "retrieve_abnf_repeat_i3";
	cases[426].test = retrieve_abnf_repeat_i3;

	cases[427].name = "retrieve_abnf_repeat_i4_dne";
	cases[427].test = retrieve_abnf_repeat_i4_dne;

	cases[428].name = "retrieve_abnf_repetition_repeat";
	cases[428].test = retrieve_abnf_repetition_repeat;

	cases[429].name = "retrieve_abnf_repetition_element";
	cases[429].test = retrieve_abnf_repetition_element;

	cases[430].name = "retrieve_abnf_concatenation_i0";
	cases[430].test = retrieve_abnf_concatenation_i0;

	cases[431].name = "retrieve_abnf_concatenation_i1";
	cases[431].test = retrieve_abnf_concatenation_i1;

	cases[432].name = "retrieve_abnf_concatenation_i2";
	cases[432].test = retrieve_abnf_concatenation_i2;

	cases[433].name = "retrieve_abnf_concatenation_i3";
	cases[433].test = retrieve_abnf_concatenation_i3;

	cases[434].name = "retrieve_abnf_concatenation_i4";
	cases[434].test = retrieve_abnf_concatenation_i4;

	cases[435].name = "retrieve_abnf_alternation_i0";
	cases[435].test = retrieve_abnf_alternation_i0;

	cases[436].name = "retrieve_abnf_alternation_i1";
	cases[436].test = retrieve_abnf_alternation_i1;

	cases[437].name = "retrieve_abnf_rule_rulename";
	cases[437].test = retrieve_abnf_rule_rulename;

	cases[438].name = "retrieve_abnf_rule_definedas";
	cases[438].test = retrieve_abnf_rule_definedas;

	cases[439].name = "retrieve_abnf_rule_elements";
	cases[439].test = retrieve_abnf_rule_elements;

	cases[440].name = "retrieve_abnf_rule_cnl";
	cases[440].test = retrieve_abnf_rule_cnl;

	size_t passes = 0;
	size_t total = 441;
	for (size_t i = 0; i < 441; i++) {
		fprintf(FOUT, "%s...", cases[i].name);
		if (cases[i].test()) {
			fprintf(FOUT, "[32m" "PASSED" "[0m" "\n");
			passes++;
		}
		else {
			fprintf(FOUT, "[31m" "FAILED" "[0m" "\n");
		}
	}
	fprintf(FOUT, "\nTesting complete (%lu/%lu passed).\n", passes, total);

	if (!finish()) {
		printf("*WARNING* POST failed to complete successfully.\n");
	}

	return 0;
}
