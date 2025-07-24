CC = gcc
CFLAGS = -Wall -Wextra

GENERATION_TARGET = generation
PROLIFIC_TARGET = prolific
SLUG_TARGET = slug
SLUGRACE_TARGET = slugrace

all: $(GENERATION_TARGET) $(PROLIFIC_TARGET) $(SLUG_TARGET) $(SLUGRACE_TARGET)

$(GENERATION_TARGET): generation.c
	$(CC) $(CFLAGS) -o $@ $^

$(PROLIFIC_TARGET): prolific.c
	$(CC) $(CFLAGS) -o $@ $^

$(SLUG_TARGET): slug.c
	$(CC) $(CFLAGS) -o $@ $^

$(SLUGRACE_TARGET): slugrace.c
	$(CC) $(CFLAGS) -o $@ $^


$(SLUGRACE_TARGET): explorer.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(GENERATION_TARGET) $(PROLIFIC_TARGET) $(SLUG_TARGET) $(SLUGRACE_TARGET)
