
char* read_line (void);
void input_files (int argc, char** argv, void (*func)(char*, FILE*, void*), void* data);
int input_flags (int argc, char* const *argv, const char* optstring, char const ** flags);
