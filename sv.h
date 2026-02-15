#ifndef CARR_SV_H_
#define CARR_SV_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>


// The user can define this macro to include only the macro functions 
// with the 'carr_' prefix, as to avoid name collisions.
// If this macro is not defined, all the versions without prefix
// will be included by default.
#ifndef CARR_SV_FORCE_PREFIX

#define StringView       CarrStringView

#define sv_from_file     carr_sv_from_file
#define sv_from_cstr     carr_sv_from_cstr
#define sv_null          carr_sv_null
#define sv_chop_by_space carr_sv_chop_by_space
#define sv_chop_line     carr_sv_chop_line
#define sv_chop_by_delim carr_sv_chop_by_delim
#define sv_strip_space   carr_sv_strip_space
#define sv_trim_right    carr_sv_trim_right
#define sv_trim_left     carr_sv_trim_left
#define sv_is_equal      carr_sv_is_equal
#define sv_printn        carr_sv_printn
#define sv_print         carr_sv_print
#define sv_to_cstr       carr_sv_to_cstr
#define sv_parse_int     carr_sv_parse_int

#endif  // CARR_SV_FORCE_PREFIX


typedef struct {
    const char* data;
    size_t      len;
} CarrStringView;

CarrStringView carr_sv_from_file(const char* file_path);
CarrStringView carr_sv_from_cstr(const char* in);
CarrStringView carr_sv_null();

CarrStringView carr_sv_chop_by_space(CarrStringView* in);
CarrStringView carr_sv_chop_line(CarrStringView* in);
CarrStringView carr_sv_chop_by_delim(CarrStringView* in, char delim);

void carr_sv_strip_space(CarrStringView* in);
void carr_sv_trim_right(CarrStringView* in, char sym);
void carr_sv_trim_left(CarrStringView* in, char sym);

bool carr_sv_is_equal(CarrStringView sv, CarrStringView other);

void carr_sv_printn(CarrStringView in);
void carr_sv_print(CarrStringView in);

char *carr_sv_to_cstr(CarrStringView in);
int   carr_sv_parse_int(CarrStringView in);

#ifdef CARR_SV_IMPLEMENTATION

CarrStringView carr_sv_from_cstr(const char* in)
{
    return (CarrStringView){
        .data = in,
        .len  = strlen(in),
    };
}

CarrStringView carr_sv_from_file(const char* file_path)
{
    FILE* f = fopen(file_path, "r");
    if (f == NULL) {
        printf(
            "%s:%d:ERROR: sv_from_file: failed to open file '%s': %s\n",
            __FILE_NAME__, __LINE__, file_path, strerror(errno)
        );
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    size_t n = ftell(f);
    fseek(f, 0, SEEK_SET);

    // TODO: memory leak...
    char* buf = (char*)malloc(n);
    fread(buf, 1, n, f);
    fclose(f);

    return (CarrStringView) {
        .data = buf,
        .len  = n,
    };
}

char *carr_sv_to_cstr(CarrStringView in)
{
    char *out = (char*)malloc((in.len + 1) * sizeof(char));
    sprintf(out, "%.*s", (int)in.len, in.data);
    return out;
}

CarrStringView carr_sv_null()
{
    return (CarrStringView){
        .data = NULL,
        .len  = 0,
    };
}

void carr_sv_print(CarrStringView in)
{
    printf("%.*s", (int)in.len, in.data);
}

void carr_sv_printn(CarrStringView in)
{
    printf("%.*s\n", (int)in.len, in.data);
}

void carr_sv_trim_left(CarrStringView* in, char sym)
{
    if (in->len == 0) {
        return;
    }

    while (*in->data == sym) {
        in->data++;
        in->len--;
    }
}

void carr_sv_trim_right(CarrStringView* in, char sym)
{
    if (in->len == 0) {
        return;
    }

    while (in->data[in->len - 1] == sym) {
        in->len--;
    }
}

void carr_sv_strip_space(CarrStringView* in)
{
    carr_sv_trim_left(in, ' ');
    carr_sv_trim_right(in, ' ');
}

bool carr_sv_is_equal(CarrStringView sv, CarrStringView other)
{
    if (sv.len != other.len) {
        return false;
    }
    for (size_t i = 0; i < sv.len; ++i) {
        if (sv.data[i] != other.data[i]) {
            return false;
        }
    }
    return true;
}

CarrStringView carr_sv_chop_by_delim(CarrStringView* in, char delim)
{
    CarrStringView out = {0};
    for (size_t i = 0; i < in->len; ++i) {
        if (in->data[i] == delim) {
            out.data = in->data;
            out.len  = i;

            in->data += i + 1;
            in->len  -= i + 1;

            return out;
        };
    }
    out = *in;
    in->data = NULL;
    in->len  = 0;
    return out;
}

CarrStringView carr_sv_chop_line(CarrStringView* in)
{
    return carr_sv_chop_by_delim(in, '\n');
}

CarrStringView carr_sv_chop_by_space(CarrStringView* in)
{
    return carr_sv_chop_by_delim(in, ' ');
}

int carr_sv_parse_int(CarrStringView in)
{
    bool is_neg = false;
    size_t start = 0;
    if (in.data[0] == '-') {
        start = 1;
        is_neg = true;
    }

    int sum = 0;
    for (size_t i = start; i < in.len; ++i) {
        char ch = in.data[i];
        if (ch < 48 || ch > 57) {
            return 0;
        }
        int n = ch - '0';
        sum *= 10;
        sum += n;
    }
    if (is_neg) {
        sum = -sum;
    }
    return sum;
}

#endif // CARR_SV_IMPLEMENTATION
#endif // CARR_SV_H_
