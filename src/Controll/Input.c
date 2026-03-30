
void input(char up, char down, char right, char left, char input, int* x, int* y)
{
    if (input == up)
    {
        *y += 1;
    }
    if (input == down)
    {
        *y -= 1;
    }
    if (input == right)
    {
        *x += 1;
    }
    if (input == left)
    {
        *x -= 1;
    }

}