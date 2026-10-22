#ifndef UNKNOWN_11C010_H
#define UNKNOWN_11C010_H

/* whether cluster row can see cluster column (the visibility bit table) */
bool function_11c010(short row, short column);

/* the same test where NONE on either side counts as visible; retail compares
   both indices as shorts before the call */
inline bool clusters_visible_11c010(short row, short column)
{
	return row == NONE || column == NONE || function_11c010(row, column);
}

#endif
