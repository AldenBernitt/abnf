#include <stdio.h>
#include "tests.c"

#define FOUT stdout

typedef struct {
	const char* name;
	_Bool(*test)();
} testcase;

int main()
{
	testcase cases[512];

	if (!setup()) {
		printf("PRE failed to complete successfully. Exiting.\n");
		return 1;
	}

	cases[0].name = "parse_char";
	cases[0].test = parse_char;

	cases[1].name = "parse_char_error_empty";
	cases[1].test = parse_char_error_empty;

	cases[2].name = "parse_char_error_invalid_character";
	cases[2].test = parse_char_error_invalid_character;

	cases[3].name = "parse_char_error_remaining_characters";
	cases[3].test = parse_char_error_remaining_characters;

	cases[4].name = "parse_char_error_too_many_characters";
	cases[4].test = parse_char_error_too_many_characters;

	cases[5].name = "parse_prose";
	cases[5].test = parse_prose;

	cases[6].name = "parse_prose_error_empty";
	cases[6].test = parse_prose_error_empty;

	cases[7].name = "parse_prose_error_invalid_character";
	cases[7].test = parse_prose_error_invalid_character;

	cases[8].name = "parse_prose_error_remaining_characters";
	cases[8].test = parse_prose_error_remaining_characters;

	cases[9].name = "parse_prose_error_too_many_characters";
	cases[9].test = parse_prose_error_too_many_characters;

	cases[10].name = "parse_chrange_lower";
	cases[10].test = parse_chrange_lower;

	cases[11].name = "parse_chrange_middle";
	cases[11].test = parse_chrange_middle;

	cases[12].name = "parse_chrange_upper";
	cases[12].test = parse_chrange_upper;

	cases[13].name = "parse_chrange_invalid_character";
	cases[13].test = parse_chrange_invalid_character;

	cases[14].name = "parse_chrange_error_empty";
	cases[14].test = parse_chrange_error_empty;

	cases[15].name = "parse_chrange_error_too_few";
	cases[15].test = parse_chrange_error_too_few;

	cases[16].name = "parse_chrange_error_too_many";
	cases[16].test = parse_chrange_error_too_many;

	cases[17].name = "parse_chmin_lower";
	cases[17].test = parse_chmin_lower;

	cases[18].name = "parse_chmin_lots";
	cases[18].test = parse_chmin_lots;

	cases[19].name = "parse_chmin_error_empty";
	cases[19].test = parse_chmin_error_empty;

	cases[20].name = "parse_chmin_error_invalid_character";
	cases[20].test = parse_chmin_error_invalid_character;

	cases[21].name = "parse_chmin_error_too_few";
	cases[21].test = parse_chmin_error_too_few;

	cases[22].name = "parse_chmax_zero";
	cases[22].test = parse_chmax_zero;

	cases[23].name = "parse_chmax_middle";
	cases[23].test = parse_chmax_middle;

	cases[24].name = "parse_chmax_upper";
	cases[24].test = parse_chmax_upper;

	cases[25].name = "parse_chmax_error_invalid_character";
	cases[25].test = parse_chmax_error_invalid_character;

	cases[26].name = "parse_chmax_error_too_many";
	cases[26].test = parse_chmax_error_too_many;

	cases[27].name = "parse_chnolim_zero";
	cases[27].test = parse_chnolim_zero;

	cases[28].name = "parse_chnolim_one";
	cases[28].test = parse_chnolim_one;

	cases[29].name = "parse_chnolim_many";
	cases[29].test = parse_chnolim_many;

	cases[30].name = "parse_chnolim_error_invalid_character_one";
	cases[30].test = parse_chnolim_error_invalid_character_one;

	cases[31].name = "parse_chnolim_error_invalid_character_many";
	cases[31].test = parse_chnolim_error_invalid_character_many;

	cases[32].name = "parse_chnolim_error_invalid_character_first";
	cases[32].test = parse_chnolim_error_invalid_character_first;

	cases[33].name = "parse_chnolim_error_invalid_character_middle";
	cases[33].test = parse_chnolim_error_invalid_character_middle;

	cases[34].name = "parse_chnolim_error_invalid_character_last";
	cases[34].test = parse_chnolim_error_invalid_character_last;

	cases[35].name = "parse_dec";
	cases[35].test = parse_dec;

	cases[36].name = "parse_dec_error_empty";
	cases[36].test = parse_dec_error_empty;

	cases[37].name = "parse_dec_error_invalid_character";
	cases[37].test = parse_dec_error_invalid_character;

	cases[38].name = "parse_dec_error_remaining_characters";
	cases[38].test = parse_dec_error_remaining_characters;

	cases[39].name = "parse_dec_error_too_many_characters";
	cases[39].test = parse_dec_error_too_many_characters;

	cases[40].name = "parse_hex";
	cases[40].test = parse_hex;

	cases[41].name = "parse_hex_error_empty";
	cases[41].test = parse_hex_error_empty;

	cases[42].name = "parse_hex_error_invalid_character";
	cases[42].test = parse_hex_error_invalid_character;

	cases[43].name = "parse_hex_error_remaining_characters";
	cases[43].test = parse_hex_error_remaining_characters;

	cases[44].name = "parse_hex_error_too_many_characters";
	cases[44].test = parse_hex_error_too_many_characters;

	cases[45].name = "parse_bin";
	cases[45].test = parse_bin;

	cases[46].name = "parse_bin_error_empty";
	cases[46].test = parse_bin_error_empty;

	cases[47].name = "parse_bin_error_invalid_character";
	cases[47].test = parse_bin_error_invalid_character;

	cases[48].name = "parse_bin_error_remaining_characters";
	cases[48].test = parse_bin_error_remaining_characters;

	cases[49].name = "parse_bin_error_too_many_characters";
	cases[49].test = parse_bin_error_too_many_characters;

	cases[50].name = "parse_zeromin_zero";
	cases[50].test = parse_zeromin_zero;

	cases[51].name = "parse_zeromin_middle";
	cases[51].test = parse_zeromin_middle;

	cases[52].name = "parse_zeromin_upper";
	cases[52].test = parse_zeromin_upper;

	cases[53].name = "parse_zeromin_error_invalid_character";
	cases[53].test = parse_zeromin_error_invalid_character;

	cases[54].name = "parse_zeromin_error_too_many";
	cases[54].test = parse_zeromin_error_too_many;

	cases[55].name = "parse_bigrange_lower";
	cases[55].test = parse_bigrange_lower;

	cases[56].name = "parse_bigrange_middle";
	cases[56].test = parse_bigrange_middle;

	cases[57].name = "parse_bigrange_upper";
	cases[57].test = parse_bigrange_upper;

	cases[58].name = "parse_bigrange_error_invalid_character";
	cases[58].test = parse_bigrange_error_invalid_character;

	cases[59].name = "parse_bigrange_error_empty";
	cases[59].test = parse_bigrange_error_empty;

	cases[60].name = "parse_bigrange_error_too_few";
	cases[60].test = parse_bigrange_error_too_few;

	cases[61].name = "parse_bigrange_error_too_many";
	cases[61].test = parse_bigrange_error_too_many;

	cases[62].name = "parse_string";
	cases[62].test = parse_string;

	cases[63].name = "parse_string_error_empty";
	cases[63].test = parse_string_error_empty;

	cases[64].name = "parse_string_error_insufficient";
	cases[64].test = parse_string_error_insufficient;

	cases[65].name = "parse_string_error_insufficient2";
	cases[65].test = parse_string_error_insufficient2;

	cases[66].name = "parse_string_error_insufficient3";
	cases[66].test = parse_string_error_insufficient3;

	cases[67].name = "parse_string_error_too_many";
	cases[67].test = parse_string_error_too_many;

	cases[68].name = "parse_string_error_repeat";
	cases[68].test = parse_string_error_repeat;

	cases[69].name = "parse_string_error_invalid_character_first";
	cases[69].test = parse_string_error_invalid_character_first;

	cases[70].name = "parse_string_error_invalid_character_middle";
	cases[70].test = parse_string_error_invalid_character_middle;

	cases[71].name = "parse_string_error_invalid_character_last";
	cases[71].test = parse_string_error_invalid_character_last;

	cases[72].name = "parse_strrange_lower";
	cases[72].test = parse_strrange_lower;

	cases[73].name = "parse_strrange_middle";
	cases[73].test = parse_strrange_middle;

	cases[74].name = "parse_strrange_upper";
	cases[74].test = parse_strrange_upper;

	cases[75].name = "parse_strrange_invalid_character";
	cases[75].test = parse_strrange_invalid_character;

	cases[76].name = "parse_strrange_error_empty";
	cases[76].test = parse_strrange_error_empty;

	cases[77].name = "parse_strrange_error_too_few";
	cases[77].test = parse_strrange_error_too_few;

	cases[78].name = "parse_strrange_error_too_many";
	cases[78].test = parse_strrange_error_too_many;

	cases[79].name = "parse_strrange_error_incomplete";
	cases[79].test = parse_strrange_error_incomplete;

	cases[80].name = "parse_strzero_empty";
	cases[80].test = parse_strzero_empty;

	cases[81].name = "parse_strzero_error_nonempty";
	cases[81].test = parse_strzero_error_nonempty;

	cases[82].name = "parse_strmin_lower";
	cases[82].test = parse_strmin_lower;

	cases[83].name = "parse_strmin_lots";
	cases[83].test = parse_strmin_lots;

	cases[84].name = "parse_strmin_error_empty";
	cases[84].test = parse_strmin_error_empty;

	cases[85].name = "parse_strmin_error_incomplete";
	cases[85].test = parse_strmin_error_incomplete;

	cases[86].name = "parse_strmin_error_invalid_character_first";
	cases[86].test = parse_strmin_error_invalid_character_first;

	cases[87].name = "parse_strmin_error_invalid_character_middle";
	cases[87].test = parse_strmin_error_invalid_character_middle;

	cases[88].name = "parse_strmin_error_invalid_character_last";
	cases[88].test = parse_strmin_error_invalid_character_last;

	cases[89].name = "parse_strmax_zero";
	cases[89].test = parse_strmax_zero;

	cases[90].name = "parse_strmax_middle";
	cases[90].test = parse_strmax_middle;

	cases[91].name = "parse_strmax_upper";
	cases[91].test = parse_strmax_upper;

	cases[92].name = "parse_strmax_error_incomplete";
	cases[92].test = parse_strmax_error_incomplete;

	cases[93].name = "parse_strmax_error_too_many";
	cases[93].test = parse_strmax_error_too_many;

	cases[94].name = "parse_strmax_error_invalid_character_first";
	cases[94].test = parse_strmax_error_invalid_character_first;

	cases[95].name = "parse_strmax_error_invalid_character_middle";
	cases[95].test = parse_strmax_error_invalid_character_middle;

	cases[96].name = "parse_strmax_error_invalid_character_last";
	cases[96].test = parse_strmax_error_invalid_character_last;

	cases[97].name = "parse_strnolim_zero";
	cases[97].test = parse_strnolim_zero;

	cases[98].name = "parse_strnolim_one";
	cases[98].test = parse_strnolim_one;

	cases[99].name = "parse_strnolim_many";
	cases[99].test = parse_strnolim_many;

	cases[100].name = "parse_strnolim_error_invalid";
	cases[100].test = parse_strnolim_error_invalid;

	cases[101].name = "parse_strnolim_error_invalid2";
	cases[101].test = parse_strnolim_error_invalid2;

	cases[102].name = "parse_strnolim_error_invalid3";
	cases[102].test = parse_strnolim_error_invalid3;

	cases[103].name = "parse_strnolim_error_invalid4";
	cases[103].test = parse_strnolim_error_invalid4;

	cases[104].name = "parse_strnolim_error_invalid5";
	cases[104].test = parse_strnolim_error_invalid5;

	cases[105].name = "parse_strnolim_error_incomplete";
	cases[105].test = parse_strnolim_error_incomplete;

	cases[106].name = "parse_concat";
	cases[106].test = parse_concat;

	cases[107].name = "parse_concat_error_empty";
	cases[107].test = parse_concat_error_empty;

	cases[108].name = "parse_concat_error_added_space";
	cases[108].test = parse_concat_error_added_space;

	cases[109].name = "parse_concat_error_invalid_order";
	cases[109].test = parse_concat_error_invalid_order;

	cases[110].name = "parse_concat_error_invalid_character";
	cases[110].test = parse_concat_error_invalid_character;

	cases[111].name = "parse_concat_error_invalid_character2";
	cases[111].test = parse_concat_error_invalid_character2;

	cases[112].name = "parse_concat_error_too_many_characters";
	cases[112].test = parse_concat_error_too_many_characters;

	cases[113].name = "parse_catrange_lower";
	cases[113].test = parse_catrange_lower;

	cases[114].name = "parse_catrange_middle";
	cases[114].test = parse_catrange_middle;

	cases[115].name = "parse_catrange_upper";
	cases[115].test = parse_catrange_upper;

	cases[116].name = "parse_catrange_error_empty";
	cases[116].test = parse_catrange_error_empty;

	cases[117].name = "parse_catrange_error_incomplete";
	cases[117].test = parse_catrange_error_incomplete;

	cases[118].name = "parse_catrange_error_too_few";
	cases[118].test = parse_catrange_error_too_few;

	cases[119].name = "parse_catrange_error_too_many";
	cases[119].test = parse_catrange_error_too_many;

	cases[120].name = "parse_catrange_error_invalid_character_first";
	cases[120].test = parse_catrange_error_invalid_character_first;

	cases[121].name = "parse_catrange_error_invalid_character_middle";
	cases[121].test = parse_catrange_error_invalid_character_middle;

	cases[122].name = "parse_catrange_error_invalid_character_last";
	cases[122].test = parse_catrange_error_invalid_character_last;

	cases[123].name = "parse_catmin_lower";
	cases[123].test = parse_catmin_lower;

	cases[124].name = "parse_catmin_lots";
	cases[124].test = parse_catmin_lots;

	cases[125].name = "parse_catmin_error_empty";
	cases[125].test = parse_catmin_error_empty;

	cases[126].name = "parse_catmin_error_incomplete";
	cases[126].test = parse_catmin_error_incomplete;

	cases[127].name = "parse_catmin_error_invalid_character_first";
	cases[127].test = parse_catmin_error_invalid_character_first;

	cases[128].name = "parse_catmin_error_invalid_character_middle";
	cases[128].test = parse_catmin_error_invalid_character_middle;

	cases[129].name = "parse_catmin_error_invalid_character_last";
	cases[129].test = parse_catmin_error_invalid_character_last;

	cases[130].name = "parse_catmax_zero";
	cases[130].test = parse_catmax_zero;

	cases[131].name = "parse_catmax_middle";
	cases[131].test = parse_catmax_middle;

	cases[132].name = "parse_catmax_upper";
	cases[132].test = parse_catmax_upper;

	cases[133].name = "parse_catmax_error_incomplete";
	cases[133].test = parse_catmax_error_incomplete;

	cases[134].name = "parse_catmax_error_too_many";
	cases[134].test = parse_catmax_error_too_many;

	cases[135].name = "parse_catmax_error_invalid_character_first";
	cases[135].test = parse_catmax_error_invalid_character_first;

	cases[136].name = "parse_catmax_error_invalid_character_middle";
	cases[136].test = parse_catmax_error_invalid_character_middle;

	cases[137].name = "parse_catmax_error_invalid_character_last";
	cases[137].test = parse_catmax_error_invalid_character_last;

	cases[138].name = "parse_catnolim_zero";
	cases[138].test = parse_catnolim_zero;

	cases[139].name = "parse_catnolim_one";
	cases[139].test = parse_catnolim_one;

	cases[140].name = "parse_catnolim_many";
	cases[140].test = parse_catnolim_many;

	cases[141].name = "parse_catnolim_error_invalid";
	cases[141].test = parse_catnolim_error_invalid;

	cases[142].name = "parse_catnolim_error_invalid2";
	cases[142].test = parse_catnolim_error_invalid2;

	cases[143].name = "parse_catnolim_error_invalid3";
	cases[143].test = parse_catnolim_error_invalid3;

	cases[144].name = "parse_catnolim_error_invalid4";
	cases[144].test = parse_catnolim_error_invalid4;

	cases[145].name = "parse_catnolim_error_invalid5";
	cases[145].test = parse_catnolim_error_invalid5;

	cases[146].name = "parse_catnolim_error_incomplete";
	cases[146].test = parse_catnolim_error_incomplete;

	cases[147].name = "parse_alt_first";
	cases[147].test = parse_alt_first;

	cases[148].name = "parse_alt_middle";
	cases[148].test = parse_alt_middle;

	cases[149].name = "parse_alt_last";
	cases[149].test = parse_alt_last;

	cases[150].name = "parse_alt_error_empty";
	cases[150].test = parse_alt_error_empty;

	cases[151].name = "parse_alt_error_invalid_character";
	cases[151].test = parse_alt_error_invalid_character;

	cases[152].name = "parse_alt_error_too_many_characters";
	cases[152].test = parse_alt_error_too_many_characters;

	cases[153].name = "parse_alt_error_too_many_characters2";
	cases[153].test = parse_alt_error_too_many_characters2;

	cases[154].name = "parse_alt_error_too_many_characters3";
	cases[154].test = parse_alt_error_too_many_characters3;

	cases[155].name = "parse_altrange_lower_first";
	cases[155].test = parse_altrange_lower_first;

	cases[156].name = "parse_altrange_lower_middle";
	cases[156].test = parse_altrange_lower_middle;

	cases[157].name = "parse_altrange_lower_last";
	cases[157].test = parse_altrange_lower_last;

	cases[158].name = "parse_altrange_lower_mixed";
	cases[158].test = parse_altrange_lower_mixed;

	cases[159].name = "parse_altrange_lower_mixed2";
	cases[159].test = parse_altrange_lower_mixed2;

	cases[160].name = "parse_altrange_lower_mixed3";
	cases[160].test = parse_altrange_lower_mixed3;

	cases[161].name = "parse_altrange_middle_first";
	cases[161].test = parse_altrange_middle_first;

	cases[162].name = "parse_altrange_middle_middle";
	cases[162].test = parse_altrange_middle_middle;

	cases[163].name = "parse_altrange_middle_last";
	cases[163].test = parse_altrange_middle_last;

	cases[164].name = "parse_altrange_middle_mixed";
	cases[164].test = parse_altrange_middle_mixed;

	cases[165].name = "parse_altrange_middle_mixed2";
	cases[165].test = parse_altrange_middle_mixed2;

	cases[166].name = "parse_altrange_middle_mixed3";
	cases[166].test = parse_altrange_middle_mixed3;

	cases[167].name = "parse_altrange_middle_mixed4";
	cases[167].test = parse_altrange_middle_mixed4;

	cases[168].name = "parse_altrange_middle_mixed5";
	cases[168].test = parse_altrange_middle_mixed5;

	cases[169].name = "parse_altrange_upper_first";
	cases[169].test = parse_altrange_upper_first;

	cases[170].name = "parse_altrange_upper_middle";
	cases[170].test = parse_altrange_upper_middle;

	cases[171].name = "parse_altrange_upper_last";
	cases[171].test = parse_altrange_upper_last;

	cases[172].name = "parse_altrange_upper_mixed";
	cases[172].test = parse_altrange_upper_mixed;

	cases[173].name = "parse_altrange_upper_mixed2";
	cases[173].test = parse_altrange_upper_mixed2;

	cases[174].name = "parse_altrange_upper_mixed3";
	cases[174].test = parse_altrange_upper_mixed3;

	cases[175].name = "parse_altrange_upper_mixed4";
	cases[175].test = parse_altrange_upper_mixed4;

	cases[176].name = "parse_altrange_upper_mixed5";
	cases[176].test = parse_altrange_upper_mixed5;

	cases[177].name = "parse_altrange_error_empty";
	cases[177].test = parse_altrange_error_empty;

	cases[178].name = "parse_altrange_error_too_few";
	cases[178].test = parse_altrange_error_too_few;

	cases[179].name = "parse_altrange_error_too_many";
	cases[179].test = parse_altrange_error_too_many;

	cases[180].name = "parse_altrange_error_invalid_character_first";
	cases[180].test = parse_altrange_error_invalid_character_first;

	cases[181].name = "parse_altrange_error_invalid_character_middle";
	cases[181].test = parse_altrange_error_invalid_character_middle;

	cases[182].name = "parse_altrange_error_invalid_character_last";
	cases[182].test = parse_altrange_error_invalid_character_last;

	cases[183].name = "parse_altmin_lower_first";
	cases[183].test = parse_altmin_lower_first;

	cases[184].name = "parse_altmin_lower_middle";
	cases[184].test = parse_altmin_lower_middle;

	cases[185].name = "parse_altmin_lower_last";
	cases[185].test = parse_altmin_lower_last;

	cases[186].name = "parse_altmin_lower_mixed";
	cases[186].test = parse_altmin_lower_mixed;

	cases[187].name = "parse_altmin_lower_mixed2";
	cases[187].test = parse_altmin_lower_mixed2;

	cases[188].name = "parse_altmin_lower_mixed3";
	cases[188].test = parse_altmin_lower_mixed3;

	cases[189].name = "parse_altmin_lots_first";
	cases[189].test = parse_altmin_lots_first;

	cases[190].name = "parse_altmin_lots_middle";
	cases[190].test = parse_altmin_lots_middle;

	cases[191].name = "parse_altmin_lots_last";
	cases[191].test = parse_altmin_lots_last;

	cases[192].name = "parse_altmin_lots_mixed";
	cases[192].test = parse_altmin_lots_mixed;

	cases[193].name = "parse_altmin_error_empty";
	cases[193].test = parse_altmin_error_empty;

	cases[194].name = "parse_altmin_error_invalid_character_first";
	cases[194].test = parse_altmin_error_invalid_character_first;

	cases[195].name = "parse_altmin_error_invalid_character_middle";
	cases[195].test = parse_altmin_error_invalid_character_middle;

	cases[196].name = "parse_altmin_error_invalid_character_last";
	cases[196].test = parse_altmin_error_invalid_character_last;

	cases[197].name = "parse_altmax_zero";
	cases[197].test = parse_altmax_zero;

	cases[198].name = "parse_altmax_middle_first";
	cases[198].test = parse_altmax_middle_first;

	cases[199].name = "parse_altmax_middle_middle";
	cases[199].test = parse_altmax_middle_middle;

	cases[200].name = "parse_altmax_middle_last";
	cases[200].test = parse_altmax_middle_last;

	cases[201].name = "parse_altmax_upper_first";
	cases[201].test = parse_altmax_upper_first;

	cases[202].name = "parse_altmax_upper_middle";
	cases[202].test = parse_altmax_upper_middle;

	cases[203].name = "parse_altmax_upper_last";
	cases[203].test = parse_altmax_upper_last;

	cases[204].name = "parse_altmax_upper_mixed";
	cases[204].test = parse_altmax_upper_mixed;

	cases[205].name = "parse_altmax_upper_mixed2";
	cases[205].test = parse_altmax_upper_mixed2;

	cases[206].name = "parse_altmax_upper_mixed3";
	cases[206].test = parse_altmax_upper_mixed3;

	cases[207].name = "parse_altmax_error_too_many";
	cases[207].test = parse_altmax_error_too_many;

	cases[208].name = "parse_altmax_error_invalid_character_first";
	cases[208].test = parse_altmax_error_invalid_character_first;

	cases[209].name = "parse_altmax_error_invalid_character_last";
	cases[209].test = parse_altmax_error_invalid_character_last;

	cases[210].name = "parse_altnolim_zero";
	cases[210].test = parse_altnolim_zero;

	cases[211].name = "parse_altnolim_first_one";
	cases[211].test = parse_altnolim_first_one;

	cases[212].name = "parse_altnolim_first_many";
	cases[212].test = parse_altnolim_first_many;

	cases[213].name = "parse_altnolim_middle_one";
	cases[213].test = parse_altnolim_middle_one;

	cases[214].name = "parse_altnolim_middle_many";
	cases[214].test = parse_altnolim_middle_many;

	cases[215].name = "parse_altnolim_last_one";
	cases[215].test = parse_altnolim_last_one;

	cases[216].name = "parse_altnolim_last_many";
	cases[216].test = parse_altnolim_last_many;

	cases[217].name = "parse_altnolim_many_mixed";
	cases[217].test = parse_altnolim_many_mixed;

	cases[218].name = "parse_altnolim_error_invalid_character";
	cases[218].test = parse_altnolim_error_invalid_character;

	cases[219].name = "parse_altnolim_error_invalid_character_first";
	cases[219].test = parse_altnolim_error_invalid_character_first;

	cases[220].name = "parse_altnolim_error_invalid_character_middle";
	cases[220].test = parse_altnolim_error_invalid_character_middle;

	cases[221].name = "parse_altnolim_error_invalid_character_last";
	cases[221].test = parse_altnolim_error_invalid_character_last;

	cases[222].name = "parse_catalt_alt1";
	cases[222].test = parse_catalt_alt1;

	cases[223].name = "parse_catalt_alt2";
	cases[223].test = parse_catalt_alt2;

	cases[224].name = "parse_catalt_error_empty";
	cases[224].test = parse_catalt_error_empty;

	cases[225].name = "parse_catalt_error_mixed";
	cases[225].test = parse_catalt_error_mixed;

	cases[226].name = "parse_catalt_error_mixed2";
	cases[226].test = parse_catalt_error_mixed2;

	cases[227].name = "parse_catalt_error_mixed3";
	cases[227].test = parse_catalt_error_mixed3;

	cases[228].name = "parse_catalt_error_mixed4";
	cases[228].test = parse_catalt_error_mixed4;

	cases[229].name = "parse_catalt_error_combined";
	cases[229].test = parse_catalt_error_combined;

	cases[230].name = "parse_catalt_error_invalid_character";
	cases[230].test = parse_catalt_error_invalid_character;

	cases[231].name = "parse_catalt_error_invalid_character2";
	cases[231].test = parse_catalt_error_invalid_character2;

	cases[232].name = "parse_catalt_error_invalid_character3";
	cases[232].test = parse_catalt_error_invalid_character3;

	cases[233].name = "parse_catalt_error_invalid_character4";
	cases[233].test = parse_catalt_error_invalid_character4;

	cases[234].name = "parse_catalt_error_duplicate";
	cases[234].test = parse_catalt_error_duplicate;

	cases[235].name = "parse_catalt_error_duplicate2";
	cases[235].test = parse_catalt_error_duplicate2;

	cases[236].name = "parse_altcat_alt1";
	cases[236].test = parse_altcat_alt1;

	cases[237].name = "parse_altcat_alt2";
	cases[237].test = parse_altcat_alt2;

	cases[238].name = "parse_altcat_error_empty";
	cases[238].test = parse_altcat_error_empty;

	cases[239].name = "parse_altcat_error_missing_alt";
	cases[239].test = parse_altcat_error_missing_alt;

	cases[240].name = "parse_altcat_error_invalid_alt";
	cases[240].test = parse_altcat_error_invalid_alt;

	cases[241].name = "parse_altcat_error_invalid_cat";
	cases[241].test = parse_altcat_error_invalid_cat;

	cases[242].name = "parse_altcat_error_invalid_cat2";
	cases[242].test = parse_altcat_error_invalid_cat2;

	cases[243].name = "parse_altcat_error_missing_cat";
	cases[243].test = parse_altcat_error_missing_cat;

	cases[244].name = "parse_altcat_error_missing_cat2";
	cases[244].test = parse_altcat_error_missing_cat2;

	cases[245].name = "parse_altcat_error_missing_cat3";
	cases[245].test = parse_altcat_error_missing_cat3;

	cases[246].name = "parse_altcat_error_missing_cat4";
	cases[246].test = parse_altcat_error_missing_cat4;

	cases[247].name = "parse_option";
	cases[247].test = parse_option;

	cases[248].name = "parse_option_empty";
	cases[248].test = parse_option_empty;

	cases[249].name = "parse_option_error_invalid";
	cases[249].test = parse_option_error_invalid;

	cases[250].name = "parse_option_error_invalid2";
	cases[250].test = parse_option_error_invalid2;

	cases[251].name = "parse_optrange_zero";
	cases[251].test = parse_optrange_zero;

	cases[252].name = "parse_optrange_one";
	cases[252].test = parse_optrange_one;

	cases[253].name = "parse_optrange_min";
	cases[253].test = parse_optrange_min;

	cases[254].name = "parse_optrange_middle";
	cases[254].test = parse_optrange_middle;

	cases[255].name = "parse_optrange_max";
	cases[255].test = parse_optrange_max;

	cases[256].name = "parse_optrange_error_too_many";
	cases[256].test = parse_optrange_error_too_many;

	cases[257].name = "parse_optrange_error_invalid";
	cases[257].test = parse_optrange_error_invalid;

	cases[258].name = "parse_optrange_error_invalid2";
	cases[258].test = parse_optrange_error_invalid2;

	cases[259].name = "parse_optrange_error_invalid3";
	cases[259].test = parse_optrange_error_invalid3;

	cases[260].name = "parse_optrange_error_invalid4";
	cases[260].test = parse_optrange_error_invalid4;

	cases[261].name = "parse_optrange_error_invalid5";
	cases[261].test = parse_optrange_error_invalid5;

	cases[262].name = "parse_optrange_error_invalid6";
	cases[262].test = parse_optrange_error_invalid6;

	cases[263].name = "parse_optmin_empty";
	cases[263].test = parse_optmin_empty;

	cases[264].name = "parse_optmin_one";
	cases[264].test = parse_optmin_one;

	cases[265].name = "parse_optmin_min";
	cases[265].test = parse_optmin_min;

	cases[266].name = "parse_optmin_some";
	cases[266].test = parse_optmin_some;

	cases[267].name = "parse_optmin_lots";
	cases[267].test = parse_optmin_lots;

	cases[268].name = "parse_optmin_error_invalid";
	cases[268].test = parse_optmin_error_invalid;

	cases[269].name = "parse_optmin_error_invalid2";
	cases[269].test = parse_optmin_error_invalid2;

	cases[270].name = "parse_optmin_error_invalid3";
	cases[270].test = parse_optmin_error_invalid3;

	cases[271].name = "parse_optmin_error_invalid4";
	cases[271].test = parse_optmin_error_invalid4;

	cases[272].name = "parse_optmin_error_invalid5";
	cases[272].test = parse_optmin_error_invalid5;

	cases[273].name = "parse_optmin_error_invalid6";
	cases[273].test = parse_optmin_error_invalid6;

	cases[274].name = "parse_optmin_error_invalid7";
	cases[274].test = parse_optmin_error_invalid7;

	cases[275].name = "parse_optmin_error_invalid8";
	cases[275].test = parse_optmin_error_invalid8;

	cases[276].name = "parse_optmin_error_invalid9";
	cases[276].test = parse_optmin_error_invalid9;

	cases[277].name = "parse_optmax_empty";
	cases[277].test = parse_optmax_empty;

	cases[278].name = "parse_optmax_one";
	cases[278].test = parse_optmax_one;

	cases[279].name = "parse_optmax_two";
	cases[279].test = parse_optmax_two;

	cases[280].name = "parse_optmax_three";
	cases[280].test = parse_optmax_three;

	cases[281].name = "parse_optmax_max";
	cases[281].test = parse_optmax_max;

	cases[282].name = "parse_optmax_error_too_many_one";
	cases[282].test = parse_optmax_error_too_many_one;

	cases[283].name = "parse_optmax_error_too_many_double";
	cases[283].test = parse_optmax_error_too_many_double;

	cases[284].name = "parse_optmax_error_too_many_lots";
	cases[284].test = parse_optmax_error_too_many_lots;

	cases[285].name = "parse_optmax_error_invalid";
	cases[285].test = parse_optmax_error_invalid;

	cases[286].name = "parse_optmax_error_invalid2";
	cases[286].test = parse_optmax_error_invalid2;

	cases[287].name = "parse_optmax_error_invalid3";
	cases[287].test = parse_optmax_error_invalid3;

	cases[288].name = "parse_optmax_error_invalid4";
	cases[288].test = parse_optmax_error_invalid4;

	cases[289].name = "parse_optmax_error_invalid5";
	cases[289].test = parse_optmax_error_invalid5;

	cases[290].name = "parse_optmax_error_invalid6";
	cases[290].test = parse_optmax_error_invalid6;

	cases[291].name = "parse_optmax_error_invalid7";
	cases[291].test = parse_optmax_error_invalid7;

	cases[292].name = "parse_optmax_error_invalid8";
	cases[292].test = parse_optmax_error_invalid8;

	cases[293].name = "parse_optmax_error_invalid9";
	cases[293].test = parse_optmax_error_invalid9;

	cases[294].name = "parse_optmax_error_invalid10";
	cases[294].test = parse_optmax_error_invalid10;

	cases[295].name = "parse_optmax_error_invalid11";
	cases[295].test = parse_optmax_error_invalid11;

	cases[296].name = "parse_optnolim_one";
	cases[296].test = parse_optnolim_one;

	cases[297].name = "parse_optnolim_empty";
	cases[297].test = parse_optnolim_empty;

	cases[298].name = "parse_optnolim_many";
	cases[298].test = parse_optnolim_many;

	cases[299].name = "parse_optnolim_error_invalid";
	cases[299].test = parse_optnolim_error_invalid;

	cases[300].name = "parse_optnolim_error_invalid2";
	cases[300].test = parse_optnolim_error_invalid2;

	cases[301].name = "parse_optnolim_error_invalid3";
	cases[301].test = parse_optnolim_error_invalid3;

	cases[302].name = "parse_optnolim_error_invalid4";
	cases[302].test = parse_optnolim_error_invalid4;

	cases[303].name = "parse_optnolim_error_invalid5";
	cases[303].test = parse_optnolim_error_invalid5;

	cases[304].name = "parse_optnolim_error_incomplete";
	cases[304].test = parse_optnolim_error_incomplete;

	cases[305].name = "parse_optgroup";
	cases[305].test = parse_optgroup;

	cases[306].name = "parse_optgroup_empty";
	cases[306].test = parse_optgroup_empty;

	cases[307].name = "parse_optgroup_many";
	cases[307].test = parse_optgroup_many;

	cases[308].name = "parse_optgroup_error_invalid";
	cases[308].test = parse_optgroup_error_invalid;

	cases[309].name = "parse_optgroup_error_invalid2";
	cases[309].test = parse_optgroup_error_invalid2;

	cases[310].name = "parse_optgroup_error_invalid3";
	cases[310].test = parse_optgroup_error_invalid3;

	cases[311].name = "parse_optgroup_error_invalid4";
	cases[311].test = parse_optgroup_error_invalid4;

	cases[312].name = "parse_optgroup_error_invalid5";
	cases[312].test = parse_optgroup_error_invalid5;

	cases[313].name = "parse_optgroup_error_incomplete";
	cases[313].test = parse_optgroup_error_incomplete;

	cases[314].name = "parse_nolimnest";
	cases[314].test = parse_nolimnest;

	cases[315].name = "parse_nolimnest_empty";
	cases[315].test = parse_nolimnest_empty;

	cases[316].name = "parse_nolimnest_many";
	cases[316].test = parse_nolimnest_many;

	cases[317].name = "parse_nolimnest_error_invalid";
	cases[317].test = parse_nolimnest_error_invalid;

	cases[318].name = "parse_nolimnest_error_invalid2";
	cases[318].test = parse_nolimnest_error_invalid2;

	cases[319].name = "parse_nolimnest_error_invalid3";
	cases[319].test = parse_nolimnest_error_invalid3;

	cases[320].name = "parse_nolimnest_error_invalid4";
	cases[320].test = parse_nolimnest_error_invalid4;

	cases[321].name = "parse_nolimnest_error_invalid5";
	cases[321].test = parse_nolimnest_error_invalid5;

	cases[322].name = "parse_nolimnest_error_incomplete";
	cases[322].test = parse_nolimnest_error_incomplete;

	cases[323].name = "parse_nolimnestopt";
	cases[323].test = parse_nolimnestopt;

	cases[324].name = "parse_nolimnestopt_empty";
	cases[324].test = parse_nolimnestopt_empty;

	cases[325].name = "parse_nolimnestopt_many";
	cases[325].test = parse_nolimnestopt_many;

	cases[326].name = "parse_nolimnestopt_error_invalid";
	cases[326].test = parse_nolimnestopt_error_invalid;

	cases[327].name = "parse_nolimnestopt_error_invalid2";
	cases[327].test = parse_nolimnestopt_error_invalid2;

	cases[328].name = "parse_nolimnestopt_error_invalid3";
	cases[328].test = parse_nolimnestopt_error_invalid3;

	cases[329].name = "parse_nolimnestopt_error_invalid4";
	cases[329].test = parse_nolimnestopt_error_invalid4;

	cases[330].name = "parse_nolimnestopt_error_invalid5";
	cases[330].test = parse_nolimnestopt_error_invalid5;

	cases[331].name = "parse_nolimnestopt_error_incomplete";
	cases[331].test = parse_nolimnestopt_error_incomplete;

	cases[332].name = "parse_decrange_first";
	cases[332].test = parse_decrange_first;

	cases[333].name = "parse_decrange_middle";
	cases[333].test = parse_decrange_middle;

	cases[334].name = "parse_decrange_last";
	cases[334].test = parse_decrange_last;

	cases[335].name = "parse_decrange_error_empty";
	cases[335].test = parse_decrange_error_empty;

	cases[336].name = "parse_decrange_error_all";
	cases[336].test = parse_decrange_error_all;

	cases[337].name = "parse_decrange_error_some";
	cases[337].test = parse_decrange_error_some;

	cases[338].name = "parse_decrange_error_some2";
	cases[338].test = parse_decrange_error_some2;

	cases[339].name = "parse_decrange_error_some3";
	cases[339].test = parse_decrange_error_some3;

	cases[340].name = "parse_decrange_error_invalid";
	cases[340].test = parse_decrange_error_invalid;

	cases[341].name = "parse_decrange_error_invalid2";
	cases[341].test = parse_decrange_error_invalid2;

	cases[342].name = "parse_hexrange_first";
	cases[342].test = parse_hexrange_first;

	cases[343].name = "parse_hexrange_middle";
	cases[343].test = parse_hexrange_middle;

	cases[344].name = "parse_hexrange_last";
	cases[344].test = parse_hexrange_last;

	cases[345].name = "parse_hexrange_error_empty";
	cases[345].test = parse_hexrange_error_empty;

	cases[346].name = "parse_hexrange_error_all";
	cases[346].test = parse_hexrange_error_all;

	cases[347].name = "parse_hexrange_error_some";
	cases[347].test = parse_hexrange_error_some;

	cases[348].name = "parse_hexrange_error_some2";
	cases[348].test = parse_hexrange_error_some2;

	cases[349].name = "parse_hexrange_error_some3";
	cases[349].test = parse_hexrange_error_some3;

	cases[350].name = "parse_hexrange_error_invalid";
	cases[350].test = parse_hexrange_error_invalid;

	cases[351].name = "parse_hexrange_error_invalid2";
	cases[351].test = parse_hexrange_error_invalid2;

	cases[352].name = "parse_binrange_first";
	cases[352].test = parse_binrange_first;

	cases[353].name = "parse_binrange_middle";
	cases[353].test = parse_binrange_middle;

	cases[354].name = "parse_binrange_last";
	cases[354].test = parse_binrange_last;

	cases[355].name = "parse_binrange_error_empty";
	cases[355].test = parse_binrange_error_empty;

	cases[356].name = "parse_binrange_error_all";
	cases[356].test = parse_binrange_error_all;

	cases[357].name = "parse_binrange_error_some";
	cases[357].test = parse_binrange_error_some;

	cases[358].name = "parse_binrange_error_some2";
	cases[358].test = parse_binrange_error_some2;

	cases[359].name = "parse_binrange_error_some3";
	cases[359].test = parse_binrange_error_some3;

	cases[360].name = "parse_binrange_error_invalid";
	cases[360].test = parse_binrange_error_invalid;

	cases[361].name = "parse_binrange_error_invalid2";
	cases[361].test = parse_binrange_error_invalid2;

	cases[362].name = "parse_deccat";
	cases[362].test = parse_deccat;

	cases[363].name = "parse_deccat_error_empty";
	cases[363].test = parse_deccat_error_empty;

	cases[364].name = "parse_deccat_error_insufficient";
	cases[364].test = parse_deccat_error_insufficient;

	cases[365].name = "parse_deccat_error_insufficient2";
	cases[365].test = parse_deccat_error_insufficient2;

	cases[366].name = "parse_deccat_error_too_many";
	cases[366].test = parse_deccat_error_too_many;

	cases[367].name = "parse_deccat_error_repeat";
	cases[367].test = parse_deccat_error_repeat;

	cases[368].name = "parse_deccat_error_invalid_character_first";
	cases[368].test = parse_deccat_error_invalid_character_first;

	cases[369].name = "parse_deccat_error_invalid_character_middle";
	cases[369].test = parse_deccat_error_invalid_character_middle;

	cases[370].name = "parse_deccat_error_invalid_character_last";
	cases[370].test = parse_deccat_error_invalid_character_last;

	cases[371].name = "parse_hexcat";
	cases[371].test = parse_hexcat;

	cases[372].name = "parse_hexcat_error_empty";
	cases[372].test = parse_hexcat_error_empty;

	cases[373].name = "parse_hexcat_error_insufficient";
	cases[373].test = parse_hexcat_error_insufficient;

	cases[374].name = "parse_hexcat_error_insufficient2";
	cases[374].test = parse_hexcat_error_insufficient2;

	cases[375].name = "parse_hexcat_error_too_many";
	cases[375].test = parse_hexcat_error_too_many;

	cases[376].name = "parse_hexcat_error_repeat";
	cases[376].test = parse_hexcat_error_repeat;

	cases[377].name = "parse_hexcat_error_invalid_character_first";
	cases[377].test = parse_hexcat_error_invalid_character_first;

	cases[378].name = "parse_hexcat_error_invalid_character_middle";
	cases[378].test = parse_hexcat_error_invalid_character_middle;

	cases[379].name = "parse_hexcat_error_invalid_character_last";
	cases[379].test = parse_hexcat_error_invalid_character_last;

	cases[380].name = "parse_bincat";
	cases[380].test = parse_bincat;

	cases[381].name = "parse_bincat_error_empty";
	cases[381].test = parse_bincat_error_empty;

	cases[382].name = "parse_bincat_error_insufficient";
	cases[382].test = parse_bincat_error_insufficient;

	cases[383].name = "parse_bincat_error_insufficient2";
	cases[383].test = parse_bincat_error_insufficient2;

	cases[384].name = "parse_bincat_error_too_many";
	cases[384].test = parse_bincat_error_too_many;

	cases[385].name = "parse_bincat_error_repeat";
	cases[385].test = parse_bincat_error_repeat;

	cases[386].name = "parse_bincat_error_invalid_character_first";
	cases[386].test = parse_bincat_error_invalid_character_first;

	cases[387].name = "parse_bincat_error_invalid_character_middle";
	cases[387].test = parse_bincat_error_invalid_character_middle;

	cases[388].name = "parse_bincat_error_invalid_character_last";
	cases[388].test = parse_bincat_error_invalid_character_last;

	cases[389].name = "parse_prosequot";
	cases[389].test = parse_prosequot;

	cases[390].name = "parse_prosequot_error_empty";
	cases[390].test = parse_prosequot_error_empty;

	cases[391].name = "parse_prosequot_error_invalid_character";
	cases[391].test = parse_prosequot_error_invalid_character;

	cases[392].name = "parse_prosequot_error_remaining_characters";
	cases[392].test = parse_prosequot_error_remaining_characters;

	cases[393].name = "parse_prosequot_error_too_many_characters";
	cases[393].test = parse_prosequot_error_too_many_characters;

	cases[394].name = "parse_quotprose";
	cases[394].test = parse_quotprose;

	cases[395].name = "parse_quotprose_error_empty";
	cases[395].test = parse_quotprose_error_empty;

	cases[396].name = "parse_quotprose_error_invalid_character";
	cases[396].test = parse_quotprose_error_invalid_character;

	cases[397].name = "parse_quotprose_error_remaining_characters";
	cases[397].test = parse_quotprose_error_remaining_characters;

	cases[398].name = "parse_quotprose_error_too_many_characters";
	cases[398].test = parse_quotprose_error_too_many_characters;

	cases[399].name = "parse_badname_error";
	cases[399].test = parse_badname_error;

	cases[400].name = "parse_name_lower";
	cases[400].test = parse_name_lower;

	cases[401].name = "parse_name_middle";
	cases[401].test = parse_name_middle;

	cases[402].name = "parse_name_upper";
	cases[402].test = parse_name_upper;

	cases[403].name = "parse_name_invalid_character";
	cases[403].test = parse_name_invalid_character;

	cases[404].name = "parse_name_error_empty";
	cases[404].test = parse_name_error_empty;

	cases[405].name = "parse_name_error_too_few";
	cases[405].test = parse_name_error_too_few;

	cases[406].name = "parse_name_error_too_many";
	cases[406].test = parse_name_error_too_many;

	cases[407].name = "parse_name_error_incomplete";
	cases[407].test = parse_name_error_incomplete;

	cases[408].name = "parse_namealt_opt1_first";
	cases[408].test = parse_namealt_opt1_first;

	cases[409].name = "parse_namealt_opt1_middle";
	cases[409].test = parse_namealt_opt1_middle;

	cases[410].name = "parse_namealt_opt1_last";
	cases[410].test = parse_namealt_opt1_last;

	cases[411].name = "parse_namealt_opt1_error_empty";
	cases[411].test = parse_namealt_opt1_error_empty;

	cases[412].name = "parse_namealt_opt1_error_all";
	cases[412].test = parse_namealt_opt1_error_all;

	cases[413].name = "parse_namealt_opt1_error_some";
	cases[413].test = parse_namealt_opt1_error_some;

	cases[414].name = "parse_namealt_opt1_error_some2";
	cases[414].test = parse_namealt_opt1_error_some2;

	cases[415].name = "parse_namealt_opt1_error_some3";
	cases[415].test = parse_namealt_opt1_error_some3;

	cases[416].name = "parse_namealt_opt1_error_invalid";
	cases[416].test = parse_namealt_opt1_error_invalid;

	cases[417].name = "parse_namealt_opt1_error_invalid2";
	cases[417].test = parse_namealt_opt1_error_invalid2;

	cases[418].name = "parse_namealt_opt2_lower";
	cases[418].test = parse_namealt_opt2_lower;

	cases[419].name = "parse_namealt_opt2_lots";
	cases[419].test = parse_namealt_opt2_lots;

	cases[420].name = "parse_namealt_opt2_error_empty";
	cases[420].test = parse_namealt_opt2_error_empty;

	cases[421].name = "parse_namealt_opt2_error_invalid_character";
	cases[421].test = parse_namealt_opt2_error_invalid_character;

	cases[422].name = "parse_namealt_opt2_error_too_few";
	cases[422].test = parse_namealt_opt2_error_too_few;

	cases[423].name = "parse_namealt_error_mixed";
	cases[423].test = parse_namealt_error_mixed;

	cases[424].name = "parse_namealt_error_mixed2";
	cases[424].test = parse_namealt_error_mixed2;

	cases[425].name = "parse_namealt_error_mixed3";
	cases[425].test = parse_namealt_error_mixed3;

	cases[426].name = "parse_namealt_error_mixed4";
	cases[426].test = parse_namealt_error_mixed4;

	cases[427].name = "parse_namealt_error_mixed5";
	cases[427].test = parse_namealt_error_mixed5;

	cases[428].name = "parse_namealt_error_mixed6";
	cases[428].test = parse_namealt_error_mixed6;

	cases[429].name = "parse_namecat_first_lower";
	cases[429].test = parse_namecat_first_lower;

	cases[430].name = "parse_namecat_first_middle";
	cases[430].test = parse_namecat_first_middle;

	cases[431].name = "parse_namecat_first_upper";
	cases[431].test = parse_namecat_first_upper;

	cases[432].name = "parse_namecat_middle_lower";
	cases[432].test = parse_namecat_middle_lower;

	cases[433].name = "parse_namecat_middle_middle";
	cases[433].test = parse_namecat_middle_middle;

	cases[434].name = "parse_namecat_middle_upper";
	cases[434].test = parse_namecat_middle_upper;

	cases[435].name = "parse_namecat_upper_lower";
	cases[435].test = parse_namecat_upper_lower;

	cases[436].name = "parse_namecat_upper_middle";
	cases[436].test = parse_namecat_upper_middle;

	cases[437].name = "parse_namecat_upper_upper";
	cases[437].test = parse_namecat_upper_upper;

	cases[438].name = "parse_namecat_error_empty";
	cases[438].test = parse_namecat_error_empty;

	cases[439].name = "parse_namecat_error_invalid";
	cases[439].test = parse_namecat_error_invalid;

	cases[440].name = "parse_namecat_error_group1_invalid";
	cases[440].test = parse_namecat_error_group1_invalid;

	cases[441].name = "parse_namecat_error_group2_invalid";
	cases[441].test = parse_namecat_error_group2_invalid;

	cases[442].name = "parse_namecat_error_group1_missing";
	cases[442].test = parse_namecat_error_group1_missing;

	cases[443].name = "parse_namecat_error_group2_missing";
	cases[443].test = parse_namecat_error_group2_missing;

	cases[444].name = "parse_namenested_opt1_first";
	cases[444].test = parse_namenested_opt1_first;

	cases[445].name = "parse_namenested_opt1_middle";
	cases[445].test = parse_namenested_opt1_middle;

	cases[446].name = "parse_namenested_opt1_last";
	cases[446].test = parse_namenested_opt1_last;

	cases[447].name = "parse_namenested_opt1_error_empty";
	cases[447].test = parse_namenested_opt1_error_empty;

	cases[448].name = "parse_namenested_opt1_error_all";
	cases[448].test = parse_namenested_opt1_error_all;

	cases[449].name = "parse_namenested_opt1_error_some";
	cases[449].test = parse_namenested_opt1_error_some;

	cases[450].name = "parse_namenested_opt1_error_some2";
	cases[450].test = parse_namenested_opt1_error_some2;

	cases[451].name = "parse_namenested_opt1_error_some3";
	cases[451].test = parse_namenested_opt1_error_some3;

	cases[452].name = "parse_namenested_opt1_error_invalid";
	cases[452].test = parse_namenested_opt1_error_invalid;

	cases[453].name = "parse_namenested_opt1_error_invalid2";
	cases[453].test = parse_namenested_opt1_error_invalid2;

	cases[454].name = "parse_namenested_opt2_lower";
	cases[454].test = parse_namenested_opt2_lower;

	cases[455].name = "parse_namenested_opt2_lots";
	cases[455].test = parse_namenested_opt2_lots;

	cases[456].name = "parse_namenested_opt2_error_empty";
	cases[456].test = parse_namenested_opt2_error_empty;

	cases[457].name = "parse_namenested_opt2_error_invalid_character";
	cases[457].test = parse_namenested_opt2_error_invalid_character;

	cases[458].name = "parse_namenested_opt2_error_too_few";
	cases[458].test = parse_namenested_opt2_error_too_few;

	cases[459].name = "parse_namenested_error_mixed";
	cases[459].test = parse_namenested_error_mixed;

	cases[460].name = "parse_namenested_error_mixed2";
	cases[460].test = parse_namenested_error_mixed2;

	cases[461].name = "parse_namenested_error_mixed3";
	cases[461].test = parse_namenested_error_mixed3;

	cases[462].name = "parse_namenested_error_mixed4";
	cases[462].test = parse_namenested_error_mixed4;

	cases[463].name = "parse_namenested_error_mixed5";
	cases[463].test = parse_namenested_error_mixed5;

	cases[464].name = "parse_namenested_error_mixed6";
	cases[464].test = parse_namenested_error_mixed6;

	cases[465].name = "parse_emptyname_empty";
	cases[465].test = parse_emptyname_empty;

	cases[466].name = "parse_emptyname_error_nonempty";
	cases[466].test = parse_emptyname_error_nonempty;

	cases[467].name = "parse_space";
	cases[467].test = parse_space;

	cases[468].name = "parse_space_error_empty";
	cases[468].test = parse_space_error_empty;

	cases[469].name = "parse_htab";
	cases[469].test = parse_htab;

	cases[470].name = "parse_htab_error_empty";
	cases[470].test = parse_htab_error_empty;

	cases[471].name = "parse_altoverlap1_opt1_one";
	cases[471].test = parse_altoverlap1_opt1_one;

	cases[472].name = "parse_altoverlap1_opt1_many";
	cases[472].test = parse_altoverlap1_opt1_many;

	cases[473].name = "parse_altoverlap1_opt2_none";
	cases[473].test = parse_altoverlap1_opt2_none;

	cases[474].name = "parse_altoverlap1_opt2_one";
	cases[474].test = parse_altoverlap1_opt2_one;

	cases[475].name = "parse_altoverlap1_opt2_many";
	cases[475].test = parse_altoverlap1_opt2_many;

	cases[476].name = "parse_altoverlap1_error_empty";
	cases[476].test = parse_altoverlap1_error_empty;

	cases[477].name = "parse_altoverlap1_error_opt2_too_many";
	cases[477].test = parse_altoverlap1_error_opt2_too_many;

	cases[478].name = "parse_altoverlap1_error_opt2_too_many2";
	cases[478].test = parse_altoverlap1_error_opt2_too_many2;

	cases[479].name = "parse_altoverlap1_error_opt2_too_many3";
	cases[479].test = parse_altoverlap1_error_opt2_too_many3;

	cases[480].name = "parse_altoverlap1_error_invalid";
	cases[480].test = parse_altoverlap1_error_invalid;

	cases[481].name = "parse_altoverlap1_error_invalid2";
	cases[481].test = parse_altoverlap1_error_invalid2;

	cases[482].name = "parse_altoverlap2_opt2_one";
	cases[482].test = parse_altoverlap2_opt2_one;

	cases[483].name = "parse_altoverlap2_opt2_many";
	cases[483].test = parse_altoverlap2_opt2_many;

	cases[484].name = "parse_altoverlap2_opt1_none";
	cases[484].test = parse_altoverlap2_opt1_none;

	cases[485].name = "parse_altoverlap2_opt1_one";
	cases[485].test = parse_altoverlap2_opt1_one;

	cases[486].name = "parse_altoverlap2_opt1_many";
	cases[486].test = parse_altoverlap2_opt1_many;

	cases[487].name = "parse_altoverlap2_error_empty";
	cases[487].test = parse_altoverlap2_error_empty;

	cases[488].name = "parse_altoverlap2_error_opt1_too_many";
	cases[488].test = parse_altoverlap2_error_opt1_too_many;

	cases[489].name = "parse_altoverlap2_error_opt1_too_many2";
	cases[489].test = parse_altoverlap2_error_opt1_too_many2;

	cases[490].name = "parse_altoverlap2_error_opt1_too_many3";
	cases[490].test = parse_altoverlap2_error_opt1_too_many3;

	cases[491].name = "parse_altoverlap2_error_invalid";
	cases[491].test = parse_altoverlap2_error_invalid;

	cases[492].name = "parse_altoverlap2_error_invalid2";
	cases[492].test = parse_altoverlap2_error_invalid2;

	cases[493].name = "parse_repoverlap_one";
	cases[493].test = parse_repoverlap_one;

	cases[494].name = "parse_repoverlap_two";
	cases[494].test = parse_repoverlap_two;

	cases[495].name = "parse_repoverlap_many";
	cases[495].test = parse_repoverlap_many;

	cases[496].name = "parse_repoverlap_error_empty";
	cases[496].test = parse_repoverlap_error_empty;

	cases[497].name = "parse_repoverlap_error_insufficient";
	cases[497].test = parse_repoverlap_error_insufficient;

	cases[498].name = "parse_repoverlap_error_norep_invalidcat";
	cases[498].test = parse_repoverlap_error_norep_invalidcat;

	cases[499].name = "parse_repoverlap_error_invalid_rep";
	cases[499].test = parse_repoverlap_error_invalid_rep;

	cases[500].name = "parse_repoverlap_error_invalid_cat";
	cases[500].test = parse_repoverlap_error_invalid_cat;

	cases[501].name = "parse_repoverlap_error_invalid_rep_cat";
	cases[501].test = parse_repoverlap_error_invalid_rep_cat;

	cases[502].name = "parse_optoverlap_no_opt";
	cases[502].test = parse_optoverlap_no_opt;

	cases[503].name = "parse_optoverlap_with_opt";
	cases[503].test = parse_optoverlap_with_opt;

	cases[504].name = "parse_optoverlap_error_empty";
	cases[504].test = parse_optoverlap_error_empty;

	cases[505].name = "parse_optoverlap_error_insufficient";
	cases[505].test = parse_optoverlap_error_insufficient;

	cases[506].name = "parse_optoverlap_error_insufficient2";
	cases[506].test = parse_optoverlap_error_insufficient2;

	cases[507].name = "parse_optoverlap_error_insufficient3";
	cases[507].test = parse_optoverlap_error_insufficient3;

	cases[508].name = "parse_optoverlap_error_invalid";
	cases[508].test = parse_optoverlap_error_invalid;

	cases[509].name = "parse_optoverlap_error_invalid2";
	cases[509].test = parse_optoverlap_error_invalid2;

	cases[510].name = "parse_optoverlap_error_invalid3";
	cases[510].test = parse_optoverlap_error_invalid3;

	cases[511].name = "parse_abnf_grammar";
	cases[511].test = parse_abnf_grammar;

	size_t passes = 0;
	size_t total = 512;
	for (size_t i = 0; i < 512; i++) {
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
