#pragma once

// SHAKE DOWN (enemy mon damaging no effect)
static int y = 0;
static int phase = 0;

if (y > 0)
{
	y--;
}
else
{
	phase++;

	if (phase == 5)
	{
		m_innerState = IS_HP_REDUCTION;
		return;
	}
	y = 15 - (3 * phase);
}

// Shake hor extended (enemy mon non-damaging move)
static int dir = true;
static int x = 0;
static int phase = 0;
static bool shouldDo = false;

if (dir)
{
	if (shouldDo)
		x++;
	shouldDo = !shouldDo;

	if (x >= 6)
	{
		dir = false;
		phase++;
	}
}
else
{
	if (shouldDo)
		x--;
	shouldDo = !shouldDo;

	if (x <= 0)
	{
		dir = true;
		phase++;
		if (phase == 4)
		{
			m_innerState = IS_MOVE_USAGE;
			x = 0;
			phase = 0;
			dir = true;
			SDL_Delay(1000);
		}
	}
}

// Shake hor heavy (enemy damaging with side effect)
static int x = 0;
static int phase = 0;

if (x > 0)
{
	x--;
}
else
{
	phase++;
	if (phase == 5)
	{
		x = 0;
		phase = 0;
		SDL_Delay(1000);
		return;
	}
	x = 15 - (3 * phase);
}

// Shake hor short (local mon non-damaging move)
static int x = 0;
static int phase = 0;

x--;
if (x <= 0)
{
	if (phase == 0 || phase == 1)
	{
		x = 6;
		phase++;
	}
	else if (phase == 2)
	{
		x = 0;
		phase = 0;
		SDL_Delay(1000);
	}
}

// Shake hor light (local damage with side effect)
static int x = 0;
static int phase = 0;
x--;
if (x <= 0)
{
	if (phase == 0 || phase == 1)
	{
		x = 3;
		phase++;
	}
	else if (phase == 2)
	{
		x = 0;
		phase = 0;
		SDL_Delay(1000);
	}
}