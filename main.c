#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef struct kort {
    const char *Vaerdi;
    const char *Kuloer;
}kort;
 
int sammenlign_to_kort(const void *p1, const void *p2);
void shuffle(kort *s, int n);
 
int main(void)
{
    kort saet[55];
    kort *kortPtr[55];
    //det her er en ændring
    int jn, in, kn;

    for(j = 0;j<55;j++) 
        kortPtr[j] = &saet[j];
 
    const char *Kuloer[] = { "Kloer", "Ruder", "Hjerte", "Spar", "joker" };
    const char *Vaerdi[] = { "Es", "To", "Tre", "Fire", "Fem", "Seks", "Syv", "Otte", "Ni", "Ti", "Knaegt", "Dronning", "Konge", " " };
     

    for(i = 0;i < 55;i++)
    {
        if(i <= 51)
        {
            saet[i].Kuloer = Kuloer[ i / 13 ];
            saet[i].Vaerdi = Vaerdi[ i % 13 ];
        }
        else
        {
            saet[i].Kuloer = Kuloer[4];
            saet[i].Vaerdi = Vaerdi[13];
        }
    }

    shuffle(saet, 55);
    printf("før sortering:\n");

    for(k = 0;k<55;k++) 
        printf("%d: %s %s\n", k + 1, kortPtr[k]->Kuloer, kortPtr[k]->Vaerdi);

    qsort(saet, 55, sizeof(struct kort), sammenlign_to_kort);
    printf("\nEfter sortering:\n");

    for(k = 0;k<55;k++) 
        printf("%d: %s %s\n", k + 1, kortPtr[k]->Kuloer, kortPtr[k]->Vaerdi);

    return 0;
}

int sammenlign_to_kort(const void *p1, const void *p2)
{
  const kort *c1 = p1;
  const kort *c2 = p2;

  int diff;

  diff = c1->Kuloer - c2->Kuloer;
  if(0 == diff)
  {
    diff = c1->Vaerdi - c2->Vaerdi;
  }

  return diff; 
  }

void shuffle(kort* s, int n)
{
    int i, j;
    kort tmp;
    srand(time(NULL));

    for(i = n - 1; i > 0; i--)
    {
        j = (rand()%54) + 1;

        tmp = s[j];
        s[j] = s[i];
        s[i] = tmp;
    }
}
