// @flags /O2 /Gr
/* UNKNOWN_0AF890.CPP: the session parameters message codecs
   (parameters-update, parameters-request, countdown-timer,
   mode-acknowledge), the registration of their message types, and the codec
   of the part both parameters messages share (0xb2330, 0xb23d0). A decoder
   returns whether the message it read is valid.

   s_message_0af890 (the encoder's view) and s_message_0b0900 (the
   decoder's) are the same parameters-update message, field for field; they
   are not merged into one structure yet */

#include "unknown_11c920.h"
#include "bitstream.h"
#include "unknown_1946f0.h"
#include "network_message_types.h"
#include <string.h>

/* every message starts with a 64 bit header */
struct s_message_header
{
	byte data[8];
};

/* one of the sixteen entries of the parameters-update message at 0x624, 0xe4 bytes */
struct s_message_0af890_entry
{
	bool present;
	bool flag;
	short count0;
	long count1;
	byte address[6];
	byte key[12];
	byte unknown1a[2];
	byte data[0xc8];
};

struct s_message_0af890
{
	byte nonce[8];
	dword value08;
	long value0c;
	bool has_a;
	bool a_flag;
	byte unknown12[2];
	dword a_count;
	dword a_value;
	bool has_b;
	byte unknown1d[3];
	dword b_value;
	bool has_c;
	byte unknown25[3];
	dword c_value;
	bool has_e;
	byte unknown2d[3];
	long e_value0;
	long e_value1;
	bool has_d;
	bool has_d2;
	byte d_data[8];
	bool has_f;
	byte unknown43;
	dword f_value;
	bool has_g;
	bool g_flag;
	bool has_h;
	byte unknown4b;
	long h_value;
	bool has_i;
	bool i_flag;
	bool has_j;
	byte unknown53;
	dword j_type;
	dword j_value0;
	dword j_value1;
	dword j_value2;
	dword j_value3;
	dword j_value4;
	dword j_value5;
	bool has_k;
	byte unknown71[3];
	dword k_value;
	bool has_l;
	bool has_l2;
	byte unknown7a[6];
	byte l_data[8];
	bool has_m;
	bool has_m2;
	byte unknown8a[2];
	byte m_data[0x308];
	bool has_n;
	byte unknown395[3];
	dword n_value0;
	dword n_value1;
	byte name[128];
	bool has_o;
	byte unknown421[7];
	byte o_data[8];
	bool has_p;
	byte unknown431[3];
	dword p_value;
	bool has_q;
	byte unknown439[3];
	dword q_value;
	bool has_r;
	byte unknown441[3];
	byte r_data[0x130];
	bool has_s;
	byte unknown575;
	word words[32];
	bool has_t;
	bool has_t2;
	dword t_mask;
	byte t_data[16][6];
	byte unknown61c[8];
	s_message_0af890_entry entries[16];
	bool has_u;
	bool has_u2;
	bool has_v;
	byte unknown1467;
	short v_value;
	bool has_w;
	bool has_w2;
	dword w_value;
	dword w_type;
	dword w_value2;
	byte w_data[12];
	bool has_x;
	bool has_x2;
	byte unknown1486[2];
	s_parameters_part x_data;
	bool has_y;
	byte unknown14cd[3];
	dword y_value;
};

// @retail 0x000af890
void __stdcall function_0af890(s_bitstream *stream, long size, s_message_0af890 *message)
{
	long i;

	function_1955d0(stream, message->nonce, 64);
	function_195720(stream, message->value08, 32);
	if (message->value0c == NONE)
	{
		stream_write_bit(stream, true);
	}
	else
	{
		stream_write_bit(stream, false);
		function_195720(stream, message->value0c, 32);
	}

	stream_write_bit(stream, message->has_a);
	if (message->has_a)
	{
		stream_write_bit(stream, message->a_flag);
		function_195720(stream, message->a_value, 32);
		stream_write_checked(stream, message->a_count, 5);
	}

	stream_write_bit(stream, message->has_b);
	if (message->has_b)
	{
		stream_write_checked(stream, message->b_value, 4);
	}

	stream_write_bit(stream, message->has_c);
	if (message->has_c)
	{
		stream_write_checked(stream, message->c_value, 2);
	}

	stream_write_bit(stream, message->has_d);
	if (message->has_d)
	{
		stream_write_bit(stream, message->has_d2);
		if (message->has_d2)
			function_1955d0(stream, message->d_data, 64);
	}

	stream_write_bit(stream, message->has_e);
	if (message->has_e)
	{
		stream_write_checked(stream, message->e_value0 - 1, 4);
		stream_write_checked(stream, message->e_value1 - 1, 4);
	}

	stream_write_bit(stream, message->has_f);
	if (message->has_f)
		stream_write_checked(stream, message->f_value, 2);

	stream_write_bit(stream, message->has_g);
	if (message->has_g)
		stream_write_bit(stream, message->g_flag);

	stream_write_bit(stream, message->has_h);
	if (message->has_h)
		stream_write_checked(stream, message->h_value + 1, 5);

	stream_write_bit(stream, message->has_i);
	if (message->has_i)
		stream_write_bit(stream, message->i_flag);

	stream_write_bit(stream, message->has_j);
	if (message->has_j)
	{
		stream_write_checked(stream, message->j_type, 3);
		dword kind = message->j_type;
		dword payload = 0;
		if (kind == 1)
		{
			stream_write_checked(stream, message->j_value0, 4);
			stream_write_checked(stream, message->j_value1, 4);
			stream_write_checked(stream, message->j_value2, 6);
			payload = message->j_value3;
		}
		else if (kind == 2)
			payload = message->j_value4;
		else if (kind == 3)
			payload = message->j_value5;
		if (kind == 1 || kind == 2 || kind == 3)
			stream_write_checked(stream, payload, 10);
	}

	stream_write_bit(stream, message->has_k);
	if (message->has_k)
		stream_write_checked(stream, message->k_value, 5);

	stream_write_bit(stream, message->has_m);
	if (message->has_m)
	{
		stream_write_bit(stream, message->has_m2);
		if (message->has_m2)
			function_063690(message->m_data, stream);
	}

	stream_write_bit(stream, message->has_l);
	if (message->has_l)
	{
		stream_write_bit(stream, message->has_l2);
		if (message->has_l2)
			function_1955d0(stream, message->l_data, 64);
	}

	stream_write_bit(stream, message->has_n);
	if (message->has_n)
	{
		function_195720(stream, message->n_value0, 32);
		function_195720(stream, message->n_value1, 32);
		i = 0;
		do
		{
			byte character = message->name[i];
			function_195720(stream, character, 8);
			if (!character)
				break;
			i++;
		} while (i < 128);
	}

	stream_write_bit(stream, message->has_o);
	if (message->has_o)
		function_1955d0(stream, message->o_data, 64);

	stream_write_bit(stream, message->has_p);
	if (message->has_p)
		function_195720(stream, message->p_value, 32);

	stream_write_bit(stream, message->has_q);
	if (message->has_q)
		stream_write_checked(stream, message->q_value, 2);

	stream_write_bit(stream, message->has_r);
	if (message->has_r)
		function_07cc50(stream, message->r_data);

	stream_write_bit(stream, message->has_s);
	if (message->has_s)
	{
		for (i = 0; i < 32; i++)
		{
			word value = message->words[i];
			function_195720(stream, value, 16);
			if (!value)
				break;
		}
	}

	stream_write_bit(stream, message->has_t);
	if (message->has_t)
	{
		stream_write_bit(stream, message->has_t2);
		if (message->has_t2)
		{
			stream_write_checked(stream, message->t_mask, 16);
			for (i = 0; i < 16; i++)
			{
				if (message->t_mask & (1 << i))
					function_1955d0(stream, message->t_data[i], 48);
			}

			for (i = 0; i < 16; i++)
			{
				s_message_0af890_entry *entry = &message->entries[i];
				stream_write_bit(stream, entry->present);
				if (entry->present)
				{
					stream_write_bit(stream, entry->flag);
					if (!entry->flag)
					{
						function_1955d0(stream, entry->address, 48);
						stream_write_checked(stream, entry->count0, 2);
						stream_write_checked(stream, entry->count1, 2);
					}
					function_1955d0(stream, entry->key, 96);
					function_07c5a0(stream, entry->data);
				}
			}
		}
	}

	stream_write_bit(stream, message->has_u);
	if (message->has_u)
		stream_write_bit(stream, message->has_u2);

	stream_write_bit(stream, message->has_v);
	if (message->has_v)
		stream_write_checked(stream, message->v_value + 1, 3);

	stream_write_bit(stream, message->has_w);
	if (message->has_w)
	{
		stream_write_bit(stream, message->has_w2);
		stream_write_checked(stream, message->w_value, 12);
		stream_write_checked(stream, message->w_type, 2);
		if (message->w_type == 1 || message->w_type == 2)
			stream_write_checked(stream, message->w_value2, 12);
		if (message->w_type == 1)
			function_1955d0(stream, message->w_data, 96);
	}

	stream_write_bit(stream, message->has_x);
	if (message->has_x)
	{
		stream_write_bit(stream, message->has_x2);
		if (message->has_x2)
			function_b2330(&message->x_data, stream);
	}

	stream_write_bit(stream, message->has_y);
	if (message->has_y)
		stream_write_checked(stream, message->y_value + 1, 5);
}

/* the decoder's view of the same entries */
struct s_message_0b0900_entry
{
	bool flag0;
	bool flag1;
	word value1;
	dword value2;
	byte address[6];
	byte unknown0e[12];
	byte unknown1a[2];
	byte unknown1c[0xc8];
};

struct s_message_0b0900
{
	s_message_header header;
	long value1;
	long value2;
	bool flag10;
	bool flag11;
	byte unknown12[2];
	long value14;
	long value18;
	bool flag1c;
	byte unknown1d[3];
	long value20;
	bool flag24;
	byte unknown25[3];
	long value28;
	bool flag2c;
	byte unknown2d[3];
	long value30;
	long value34;
	bool flag38;
	bool flag39;
	byte data3a[8];
	bool flag42;
	byte unknown43;
	long value44;
	bool flag48;
	bool flag49;
	bool flag4a;
	byte unknown4b;
	long value4c;
	bool flag50;
	bool flag51;
	bool flag52;
	byte unknown53;
	long value54;
	long value58;
	long value5c;
	long value60;
	long value64;
	long value68;
	long value6c;
	bool flag70;
	byte unknown71[3];
	long value74;
	bool flag78;
	bool flag79;
	byte unknown7a[6];
	byte data80[8];
	bool flag88;
	bool flag89;
	byte unknown8a[2];
	byte data8c[0x308];
	bool flag394;
	byte unknown395[3];
	long value398;
	long value39c;
	byte string[0x80];
	bool flag420;
	byte unknown421[7];
	byte data428[8];
	bool flag430;
	byte unknown431[3];
	long value434;
	bool flag438;
	byte unknown439[3];
	long value43c;
	bool flag440;
	byte unknown441[3];
	byte data444[0x130];
	bool flag574;
	byte unknown575;
	word words[32];
	bool flag5b6;
	bool flag5b7;
	dword mask;
	byte addresses[16][6];
	byte unknown61c[8];
	s_message_0b0900_entry entries[16];
	bool flag1464;
	bool flag1465;
	bool flag1466;
	byte unknown1467;
	short value1468;
	bool flag146a;
	bool flag146b;
	long value146c;
	long value1470;
	long value1474;
	byte data1478[12];
	bool flag1484;
	bool flag1485;
	byte unknown1486[2];
	s_parameters_part data1488;
	bool flag14cc;
	byte unknown14cd[3];
	long value14d0;
};

// @retail 0xb0900
bool __stdcall function_0b0900(s_bitstream *stream, long unused, s_message_0b0900 *message)
{
	bool valid = true;
	function_195820(stream, message, 0x40);
	message->value1 = function_1959c0(stream, 0x20);
	if (function_1957d0(stream))
		message->value2 = NONE;
	else
		message->value2 = function_1959c0(stream, 0x20);
	message->flag10 = function_1957d0(stream);
	if (message->flag10)
	{
		message->flag11 = function_1957d0(stream);
		message->value18 = function_1959c0(stream, 0x20);
		message->value14 = function_1959c0(stream, 5);
	}
	message->flag1c = function_1957d0(stream);
	if (message->flag1c)
		message->value20 = function_1959c0(stream, 4);
	message->flag24 = function_1957d0(stream);
	if (message->flag24)
		message->value28 = function_1959c0(stream, 2);
	message->flag38 = function_1957d0(stream);
	if (message->flag38)
	{
		message->flag39 = function_1957d0(stream);
		if (message->flag39)
			function_195820(stream, message->data3a, 0x40);
	}
	message->flag2c = function_1957d0(stream);
	if (message->flag2c)
	{
		message->value30 = function_1959c0(stream, 4) + 1;
		message->value34 = function_1959c0(stream, 4) + 1;
	}
	message->flag42 = function_1957d0(stream);
	if (message->flag42)
		message->value44 = function_1959c0(stream, 2);
	message->flag48 = function_1957d0(stream);
	if (message->flag48)
		message->flag49 = function_1957d0(stream);
	message->flag4a = function_1957d0(stream);
	if (message->flag4a)
		message->value4c = function_1959c0(stream, 5) - 1;
	message->flag50 = function_1957d0(stream);
	if (message->flag50)
		message->flag51 = function_1957d0(stream);
	message->flag52 = function_1957d0(stream);
	if (message->flag52)
	{
		message->value54 = function_1959c0(stream, 3);
		if (message->value54 == 1)
		{
			message->value58 = function_1959c0(stream, 4);
			message->value5c = function_1959c0(stream, 4);
			message->value60 = function_1959c0(stream, 6);
			message->value64 = function_1959c0(stream, 10);
		}
		else if (message->value54 == 2)
			message->value68 = function_1959c0(stream, 10);
		else if (message->value54 == 3)
			message->value6c = function_1959c0(stream, 10);
	}
	message->flag70 = function_1957d0(stream);
	if (message->flag70)
		message->value74 = function_1959c0(stream, 5);
	message->flag88 = function_1957d0(stream);
	if (message->flag88)
	{
		message->flag89 = function_1957d0(stream);
		if (message->flag89)
			valid = function_063980(stream, message->data8c);
	}
	message->flag78 = function_1957d0(stream);
	if (message->flag78)
	{
		message->flag79 = function_1957d0(stream);
		if (message->flag79)
			function_195820(stream, message->data80, 0x40);
	}
	message->flag394 = function_1957d0(stream);
	if (message->flag394)
	{
		message->value398 = function_1959c0(stream, 0x20);
		message->value39c = function_1959c0(stream, 0x20);
		read_byte_string(stream, message->string, 0x80);
	}
	message->flag420 = function_1957d0(stream);
	if (message->flag420)
		function_195820(stream, message->data428, 0x40);
	message->flag430 = function_1957d0(stream);
	if (message->flag430)
		message->value434 = function_1959c0(stream, 0x20);
	message->flag438 = function_1957d0(stream);
	if (message->flag438)
		message->value43c = function_1959c0(stream, 2);
	message->flag440 = function_1957d0(stream);
	if (message->flag440)
		valid = valid && function_07d520(stream, message->data444);
	message->flag574 = function_1957d0(stream);
	if (message->flag574)
		function_194fa0(stream, message->words, 0x20);
	message->flag5b6 = function_1957d0(stream);
	if (message->flag5b6)
	{
		message->flag5b7 = function_1957d0(stream);
		if (message->flag5b7)
		{
			message->mask = function_1959c0(stream, 0x10);
			for (long address_index = 0; address_index < 16; address_index++)
			{
				if (message->mask & (1 << address_index))
				{
					long j;
					function_195820(stream, message->addresses[address_index], 0x30);
					for (j = 0; j < address_index; j++)
					{
						if ((message->mask & (1 << j)) && memcmp(message->addresses[address_index], message->addresses[j], 6) == 0)
							valid = false;
					}
				}
			}
			for (long entry_index = 0; entry_index < 16; entry_index++)
			{
				message->entries[entry_index].flag0 = stream_read_bit(stream);
				if (message->entries[entry_index].flag0)
				{
					message->entries[entry_index].flag1 = stream_read_bit(stream);
					if (message->entries[entry_index].flag1)
					{
						message->entries[entry_index].value1 = (word)NONE;
						message->entries[entry_index].value2 = NONE;
					}
					else
					{
						function_195820(stream, message->entries[entry_index].address, 0x30);
						message->entries[entry_index].value1 = (word)function_1959c0(stream, 2);
						message->entries[entry_index].value2 = function_1959c0(stream, 2);
					}
					function_195820(stream, message->entries[entry_index].unknown0e, 0x60);
					bool entry_valid = function_07ca70(stream, message->entries[entry_index].unknown1c);
					valid = valid && entry_valid;
					if (valid)
					{
						long j;
						if (!message->entries[entry_index].flag1)
						{
							long found = NONE;
							for (j = 0; j < 16; j++)
							{
								if ((message->mask & (1 << j)) && memcmp(message->addresses[j], message->entries[entry_index].address, 6) == 0)
									found = j;
							}
							valid = found != NONE;
						}
						for (j = 0; j < entry_index; j++)
						{
							s_message_0b0900_entry *other = &message->entries[j];
							if (other->flag0)
							{
								if (valid && memcmp(message->entries[entry_index].unknown0e, other->unknown0e, 12) == 0)
									valid = false;
								if (!message->entries[entry_index].flag1)
								{
									if (memcmp(message->entries[entry_index].address, other->address, 6) == 0)
										valid = valid && message->entries[entry_index].value1 != other->value1 && message->entries[entry_index].value2 != other->value2;
								}
							}
						}
					}
				}
			}
		}
	}
	message->flag1464 = stream_read_bit(stream);
	if (message->flag1464)
		message->flag1465 = stream_read_bit(stream);
	message->flag1466 = stream_read_bit(stream);
	if (message->flag1466)
	{
		message->value1468 = (short)(function_1959c0(stream, 3) - 1);
		if (valid && (message->value1468 == NONE || (message->value1468 >= 0 && message->value1468 < 4)))
			valid = true;
		else
			valid = false;
	}
	message->flag146a = stream_read_bit(stream);
	if (message->flag146a)
	{
		message->flag146b = stream_read_bit(stream);
		message->value146c = function_1959c0(stream, 12);
		message->value1470 = function_1959c0(stream, 2);
		if (message->value1470 == 1 || message->value1470 == 2)
			message->value1474 = function_1959c0(stream, 12);
		else
			message->value1474 = NONE;
		if (message->value1470 == 1)
			function_195820(stream, message->data1478, 0x60);
	}
	message->flag1484 = stream_read_bit(stream);
	if (message->flag1484)
	{
		message->flag1485 = stream_read_bit(stream);
		if (message->flag1485)
        {
            bool decoded = function_b23d0(stream, &message->data1488);
            valid = valid && decoded;
        }
	}
	message->flag14cc = stream_read_bit(stream);
	if (message->flag14cc)
		message->value14d0 = function_1959c0(stream, 5) - 1;
	valid = valid && !stream_overflowed(stream) && (message->value2 == NONE || message->value2 < message->value1);
	if (message->flag10)
		valid = valid && message->value14 > 0 && message->value14 < 19 && message->value18 >= 0;
	if (message->flag1c)
		valid = valid && message->value20 >= 0 && message->value20 < 9;
	if (message->flag24)
		valid = valid && message->value28 >= 0 && message->value28 < 3;
	if (message->flag2c)
		valid = valid && message->value30 > 0 && message->value30 <= 16 && message->value34 > 0 && message->value34 <= 16;
	if (message->flag42)
		valid = valid && message->value44 >= 0 && message->value44 < 3;
	if (message->flag4a)
		valid = valid && (message->value4c == NONE || (message->value4c >= 0 && message->value4c < 16));
	if (message->flag52)
	{
		valid = valid && message->value54 >= 0 && message->value54 < 5;
		if (message->value54 == 1)
			valid = valid && message->value58 >= 0 && message->value58 <= 15 && message->value5c >= 0 && message->value5c <= 15 && message->value60 >= 0 && message->value60 <= 63 && message->value64 >= 0 && message->value64 <= 1023;
	}
	if (message->flag70)
		valid = valid && message->value74 >= 0 && message->value74 < 20;
	if (message->flag438)
		valid = valid && message->value43c >= 0 && message->value43c < 3;
	if (message->flag146a)
	{
		valid = valid && message->value146c >= 0 && message->value1470 >= 0 && message->value1470 < 3;
		if (message->value1470 == 1 || message->value1470 == 2)
			valid = valid && message->value1474 >= 0;
		else
			valid = valid && message->value1474 == NONE;
	}
	if (message->flag14cc)
	{
		long v = message->value14d0;
		valid = valid && ((v < 0 ? 0 : (v > 15 ? 15 : v)) == v || v == NONE);
	}
	return valid;
}

/* the parameters-request message, 0x59c bytes: a run of optional fields,
   each preceded by a flag */
struct s_message_parameters_request
{
	s_message_header header;
	bool has_value0;
	long value0;
	bool has_value1;
	long value1;
	bool has_value2;
	long value2;
	bool has_value3;
	long value3;
	bool has_flag4;
	bool flag4;
	bool has_value5;
	long value5;
	bool has_flag6;
	bool flag6;
	bool has_value7;
	long value7;
	bool has_part8;
	bool part8_valid;
	dword part8[0xc2];
	bool has_name;
	dword name_value0;
	dword name_value1;
	byte name[0x80];
	bool has_value10;
	long value10;
	bool has_part11;
	dword part11[0x4c];
	bool has_words;
	word words[0x20];
	bool has_flag13;
	bool flag13;
	bool has_value14;
	short value14;
	bool has_part15;
	bool part15_valid;
	s_parameters_part part15;
};

// @retail 0xb14a0
void __stdcall function_b14a0(s_bitstream *stream, long size, void *message_)
{
	s_message_parameters_request *message = (s_message_parameters_request *)message_;

	function_1955d0(stream, message, 0x40);
	stream_write_bit(stream, message->has_value0);
	if (message->has_value0)
		stream_write_checked(stream, message->value0, 5);
	stream_write_bit(stream, message->has_value1);
	if (message->has_value1)
		stream_write_checked(stream, message->value1, 4);
	stream_write_bit(stream, message->has_value2);
	if (message->has_value2)
		stream_write_checked(stream, message->value2, 2);
	stream_write_bit(stream, message->has_value3);
	if (message->has_value3)
		stream_write_checked(stream, message->value3, 2);
	stream_write_bit(stream, message->has_flag4);
	if (message->has_flag4)
		stream_write_bit(stream, message->flag4);
	stream_write_bit(stream, message->has_value5);
	if (message->has_value5)
		stream_write_checked(stream, message->value5 + 1, 5);
	stream_write_bit(stream, message->has_flag6);
	if (message->has_flag6)
		stream_write_bit(stream, message->flag6);
	stream_write_bit(stream, message->has_value7);
	if (message->has_value7)
		stream_write_checked(stream, message->value7, 5);
	stream_write_bit(stream, message->has_part8);
	if (message->has_part8)
	{
		stream_write_bit(stream, message->part8_valid);
		if (message->part8_valid)
			function_063690(message->part8, stream);
	}
	stream_write_bit(stream, message->has_name);
	if (message->has_name)
	{
		function_195720(stream, message->name_value0, 32);
		function_195720(stream, message->name_value1, 32);
		for (long i = 0; i < 0x80; i++)
		{
			byte c = message->name[i];
			function_195720(stream, c, 8);
			if (c == 0)
				break;
		}
	}
	stream_write_bit(stream, message->has_value10);
	if (message->has_value10)
		stream_write_checked(stream, message->value10, 2);
	stream_write_bit(stream, message->has_part11);
	if (message->has_part11)
		function_07cc50(stream, message->part11);
	stream_write_bit(stream, message->has_words);
	if (message->has_words)
	{
		for (long i = 0; i < 0x20; i++)
		{
			word w = message->words[i];
			function_195720(stream, w, 16);
			if (w == 0)
				break;
		}
	}
	stream_write_bit(stream, message->has_flag13);
	if (message->has_flag13)
		stream_write_bit(stream, message->flag13);
	stream_write_bit(stream, message->has_value14);
	if (message->has_value14)
		stream_write_checked(stream, message->value14 + 1, 3);
	stream_write_bit(stream, message->has_part15);
	if (message->has_part15)
	{
		stream_write_bit(stream, message->part15_valid);
		if (message->part15_valid)
			function_b2330(&message->part15, stream);
	}
}

// @retail 0xb1c80
bool __stdcall function_b1c80(s_bitstream *stream, long size, void *message_)
{
	s_message_parameters_request *message = (s_message_parameters_request *)message_;
	bool valid = true;

	function_195820(stream, message, 0x40);
	message->has_value0 = function_1957d0(stream);
	if (message->has_value0)
		message->value0 = function_1959c0(stream, 5);
	message->has_value1 = function_1957d0(stream);
	if (message->has_value1)
		message->value1 = function_1959c0(stream, 4);
	message->has_value2 = function_1957d0(stream);
	if (message->has_value2)
		message->value2 = function_1959c0(stream, 2);
	message->has_value3 = function_1957d0(stream);
	if (message->has_value3)
		message->value3 = function_1959c0(stream, 2);
	message->has_flag4 = function_1957d0(stream);
	if (message->has_flag4)
		message->flag4 = function_1957d0(stream);
	message->has_value5 = function_1957d0(stream);
	if (message->has_value5)
		message->value5 = function_1959c0(stream, 5) - 1;
	message->has_flag6 = function_1957d0(stream);
	if (message->has_flag6)
		message->flag6 = function_1957d0(stream);
	message->has_value7 = function_1957d0(stream);
	if (message->has_value7)
		message->value7 = function_1959c0(stream, 5);
	message->has_part8 = function_1957d0(stream);
	if (message->has_part8)
	{
		message->part8_valid = function_1957d0(stream);
		if (message->part8_valid)
		{
			byte const &decoded = function_063980(stream, message->part8);
			valid = decoded != 0;
		}
	}
	message->has_name = function_1957d0(stream);
	if (message->has_name)
	{
		message->name_value0 = function_1959c0(stream, 32);
		message->name_value1 = function_1959c0(stream, 32);
		read_byte_string(stream, message->name, 0x80);
	}
	message->has_value10 = function_1957d0(stream);
	if (message->has_value10)
		message->value10 = function_1959c0(stream, 2);
	message->has_part11 = function_1957d0(stream);
	if (message->has_part11)
		valid = valid && function_07d520(stream, message->part11);
	message->has_words = function_1957d0(stream);
	if (message->has_words)
		function_194fa0(stream, message->words, 0x20);
	message->has_flag13 = function_1957d0(stream);
	if (message->has_flag13)
		message->flag13 = function_1957d0(stream);
	message->has_value14 = function_1957d0(stream);
	if (message->has_value14)
	{
		message->value14 = (short)(function_1959c0(stream, 3) - 1);
		valid = valid && (message->value14 == -1 || (message->value14 >= 0 && message->value14 < 4));
	}
	message->has_part15 = function_1957d0(stream);
	if (message->has_part15)
	{
		message->part15_valid = function_1957d0(stream);
		if (message->part15_valid)
		{
			bool decoded = function_b23d0(stream, &message->part15);
			valid = valid && decoded;
		}
	}
	bool result = valid && !stream_overflowed(stream);
	if (message->has_value0)
		result = result && message->value0 > 0 && message->value0 < 0x13;
	if (message->has_value1)
		result = result && message->value1 >= 0 && message->value1 < 9;
	if (message->has_value2)
		result = result && message->value2 >= 0 && message->value2 < 3;
	if (message->has_value3)
		result = result && message->value3 >= 0 && message->value3 < 3;
	if (message->has_value5)
		result = result && (message->value5 == NONE || (message->value5 >= 0 && message->value5 < 0x10));
	if (message->has_value7)
		result = result && message->value7 >= 0 && message->value7 < 0x14;
	if (message->has_value10)
		result = result && message->value10 >= 0 && message->value10 < 3;
	return result;
}

/* the countdown-timer message, 0x20 bytes */
struct s_message_countdown_timer
{
	s_message_header header;
	bool flag;
	long value;
	long kind;
	byte data[0xc];
};

// @retail 0xb1fd0
void __stdcall function_b1fd0(s_bitstream *stream, long size, void *message_)
{
	s_message_countdown_timer *message = (s_message_countdown_timer *)message_;

	function_1955d0(stream, message, 0x40);
	stream_write_bit(stream, message->flag);
	stream_write_checked(stream, message->value, 12);
	stream_write_checked(stream, message->kind, 2);
	if (message->kind == 1)
		function_1955d0(stream, message->data, 0x60);
}

// @retail 0xb20c0
bool __stdcall function_b20c0(s_bitstream *stream, long size, void *message_)
{
	s_message_countdown_timer *message = (s_message_countdown_timer *)message_;

	function_195820(stream, message, 0x40);
	message->flag = function_1957d0(stream);
	message->value = function_1959c0(stream, 12);
	message->kind = function_1959c0(stream, 2);
	if (message->kind == 1)
		function_195820(stream, message->data, 0x60);
	if (!stream_overflowed(stream) && message->value >= 0 && message->kind >= 0 && message->kind < 3)
		return true;
	return false;
}

/* the mode-acknowledge message, 0x10 bytes */
struct s_message_mode_acknowledge
{
	s_message_header header;
	long mode;
	long value;
};

// @retail 0xb2140
void __stdcall function_b2140(s_bitstream *stream, long size, void *message_)
{
	s_message_mode_acknowledge *message = (s_message_mode_acknowledge *)message_;

	function_1955d0(stream, message, 0x40);
	stream_write_checked(stream, message->mode, 5);
	function_195720(stream, message->value, 32);
}

// @retail 0xb21b0
bool __stdcall function_b21b0(s_bitstream *stream, long size, void *message_)
{
	s_message_mode_acknowledge *message = (s_message_mode_acknowledge *)message_;

	function_195820(stream, message, 0x40);
	message->mode = function_1959c0(stream, 5);
	message->value = function_1959c0(stream, 32);
	if (!stream_overflowed(stream) && message->mode > 0 && message->mode < 0x13 && message->value >= 0)
		return true;
	return false;
}

// @retail 0xb2220
void network_message_types_register_session_parameters(c_type_659ceb *collection)
{
	REGISTER_MESSAGE_TYPE(collection, _network_message_type_parameters_update, "parameters-update", 0x14d8, function_0af890, function_0b0900);
	REGISTER_MESSAGE_TYPE(collection, _network_message_type_parameters_request, "parameters-request", 0x59c, function_b14a0, function_b1c80);
	REGISTER_MESSAGE_TYPE(collection, _network_message_type_countdown_timer, "countdown-timer", 0x20, function_b1fd0, function_b20c0);
	REGISTER_MESSAGE_TYPE(collection, _network_message_type_mode_acknowledge, "mode-acknowledge", 0x10, function_b2140, function_b21b0);
}

// @retail 0xb2330
void function_b2330(s_parameters_part *part, s_bitstream *stream)
{
	stream_write_checked(stream, part->unknown00, 1);
	function_1955d0(stream, part->unknown04, 0x40);
	function_1955d0(stream, part->unknown0c, 0x80);
	function_1955d0(stream, part->unknown1c, 0x120);
	stream_write_checked(stream, part->unknown40, 2);
}

// @retail 0xb23d0
bool function_b23d0(s_bitstream *stream, s_parameters_part *part)
{
	part->unknown00 = function_1959c0(stream, 1);
	function_195820(stream, part->unknown04, 0x40);
	function_195820(stream, part->unknown0c, 0x80);
	function_195820(stream, part->unknown1c, 0x120);
	part->unknown40 = function_1959c0(stream, 2);
	if (part->unknown00 >= 0 && part->unknown00 < 2 && part->unknown40 >= 0 && part->unknown40 < 3)
		return true;
	return false;
}
