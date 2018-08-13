#include <stdio.h>
#include <string.h>

typedef _Bool bool;
#define TRUE ( 0 )
#define FALSE ( 1 )

typedef struct
{
  char choiceText[96];
  int choiceNodeNumber;
} choice_t;

typedef struct
{
  int nodeNumber;
  char displayText[4096];
  int numberOfChoices;
  choice_t choices[10];
} nodeEntry_t;

nodeEntry_t nodeTable[] =
{
  {
    1,
    "Hello there, would you like to play a game?",
    2,
    {
      {"no", 2},
      {"yes", 3}
    }
  },
  {
    2,
    "Okay then, see you later sucker...!!!",
    0,
    {}
  },
  {
    3,
    "Good choice. Now to the story! You wake up in an abandoned mansion. You are in a corridor, you do not know how you got there. Do you want to go left or right?",
    2,
    {
      {"left", 4},
      {"right", 7}
    }
  },
  {
    4,
    "You start walking and hear a noise behind you. Do you, go back or keep going?",
    2,
    {
      {"go back", 7},
      {"keep going", 6}
    }
  },
  {
    7,
    "You start walking. Suddenly you bump into a full grown man wearing a duck mask with several cracks in it!! All of a sudden, he makes a loud noise!! Do you run away or fight?",
    2,
    {
      {"run away", 6},
      {"fight", 9}
    }
  },
  {
    9,
    "You get a fist to the face, and you are knocked out!! You wake up in a closet wearing a sheep mask with a few cracks.",
    0,
    {}
  },
  {
    6,
    "You find a room and go inside. There you see a bunch of animal masks, from ducks to cows to moose. Do you investigate or leave?",
    2,
    {
      {"investigate", 11},
      {"leave", 12}
    }
  },
  {
    11,
    "After looking around the room, you come across a mask that really stands out. Behind it, you find a lever. Do you pull the lever or look around more?",
    2,
    {
      {"pull lever", 13},
      {"look more", 14}
    }
  },
  {
    12,
    "On the way out of the door, you suddenly bump into the duck man!! He says \"Assistant I need, an assistant you are. If you disagree, you gone.\" You suddenly find yourself waking up in a different room wearing a cow mask!",
    0,
    {}
  },
  {
    14,
    "You don't find anything. Do you pull the lever or exit the room?",
    2,
    {
      {"pull lever", 13},
      {"exit", 12}
    }
  },
  {
    13,
    "Suddenly, the room starts shaking and you fall on the ground! You see a door appear across the room. Do you enter the door?",
    2,
    {
      {"enter", 15},
      {"not enter", 16}
    }
  },

};

int askForTwoChoices (char *choiceOne,
		      char *choiceTwo)
{
  bool validChoice = FALSE;
  char answer[1000];

  printf ("\n   Do you choose %s (\"%c\") or %s (\"%c\") ",
	  choiceOne,
	  choiceOne[0],
	  choiceTwo,
	  choiceTwo[0]);

  do
  {
    memset (answer, 0, 1000);
    scanf ("%s",answer);

    if ((0 == strncmp (answer, choiceOne, 1)) || 
        (0 == strncmp (answer, choiceTwo, 1)))
    {
      validChoice = TRUE;
    }
    else
    {
      printf ("\n   Remember, you can only type (\"%c\") or (\"%c\") ",
	      choiceOne[0],
	      choiceTwo[0]);
    }
  }
  while (FALSE == validChoice);

  if (0 == strncmp (answer, choiceOne, 1))
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

void displayText (char *text)
{
  printf ("\n%s\n", text);
}

bool findNode (int         inputNodeRef,
               nodeEntry_t **tableEntry)
{
  int         tableSize = (sizeof (nodeTable) / sizeof (nodeEntry_t));
  nodeEntry_t *entry;
  int         i = 0;

  /* Initialize for error. */
  *tableEntry = NULL;

  while (i<tableSize)
  {
    entry = &(nodeTable[i]);

    if (inputNodeRef == entry->nodeNumber)
    {
      break;
    }
    i++;
  }

  if (i == tableSize)
  {
    return FALSE;
  }

  *tableEntry = entry;
  return TRUE;
}

int processNode (int inputNodeRef)
{
  nodeEntry_t *tableEntry;
  int         answer = 0;

  if (FALSE == findNode (inputNodeRef, &tableEntry))
  {
    displayText ("You have reached the end of the line... ");
    printf("(Failed to find node %d)\n", inputNodeRef);
    return 0;
  }

  displayText (tableEntry->displayText);

  if (0 == tableEntry->numberOfChoices)
  {
    return 0;
  }

  answer = askForTwoChoices (tableEntry->choices[0].choiceText,
                             tableEntry->choices[1].choiceText);

  return tableEntry->choices[answer].choiceNodeNumber;
}

void auditTableForDuplicates (void)
{
  int         tableSize = (sizeof (nodeTable) / sizeof (nodeEntry_t));
  nodeEntry_t *entry1;
  nodeEntry_t *entry2;
  int         i = 0;
  int         j = 0;

  while (i<tableSize)
  {
    entry1 = &(nodeTable[i]);

    j = i + 1;

    while (j<tableSize)
    {
      entry2 = &(nodeTable[j]);

      if (entry1->nodeNumber == entry2->nodeNumber)
      {
        printf ("\n\n ### Error in table definition: duplicate entries for reference %d. ###\n\n",
                entry1->nodeNumber);
      }
      j++;
    }
    i++;
  }

  return;
}

void auditTableForDanglingReferences (void)
{
  int         tableSize = (sizeof (nodeTable) / sizeof (nodeEntry_t));
  nodeEntry_t *entry1;
  nodeEntry_t *entry2;
  int         i = 0;
  int         j = 0;

  printf ("\n\n");

  for (i=0; i<tableSize; i++)
  {
    entry1 = &(nodeTable[i]);

    for (j=0; j<entry1->numberOfChoices; j++)
    {
      if (FALSE == findNode (entry1->choices[j].choiceNodeNumber, &entry2))
      {
        printf (" ### Error in table definition: dangling reference %d. ###\n",
                entry1->choices[j].choiceNodeNumber);
      }
    }
  }

  printf ("\n");

  return;
}

void main ()
{
  int node    = 1;
  int newNode = 0;

  auditTableForDuplicates ();

  auditTableForDanglingReferences ();

  while (0 != node)
  {
    newNode = processNode (node);
    node = newNode;
  }

  displayText ("Game over.");
  return;
}
