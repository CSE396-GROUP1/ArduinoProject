#include <Maze.h>
#include <string.h>

typedef enum DIRECTION
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

DIRECTION direction;

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);
/*
static const unsigned char PROGMEM logo [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00,
0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x06, 0x80, 0x00, 0x00, 0x3C, 0x00, 0x1C, 0x00, 0x00, 0x00,
0xEC, 0x00, 0xF0, 0x00, 0x00, 0x00, 0xF8, 0x07, 0xE0, 0x00, 0x00, 0x00, 0xF8, 0x1F, 0xC0, 0x00,
0x00, 0x03, 0xD0, 0x7F, 0x00, 0x00, 0x00, 0x01, 0x81, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFC,
0x00, 0x00, 0x00, 0x0C, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x08, 0x3F, 0xF8, 0x00, 0x00, 0x00, 0x00,
0xFD, 0x20, 0x00, 0x00, 0x00, 0x13, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x82, 0x11, 0xE0, 0x00, 0x00,
0x00, 0x84, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x11, 0xE3, 0x80, 0x00, 0x00, 0x00, 0x89, 0xA2, 0x00,
0x00, 0x00, 0x00, 0xDA, 0x64, 0x80, 0x00, 0x00, 0x00, 0x86, 0x19, 0x80, 0x00, 0x00, 0x00, 0x91,
0xC2, 0x00, 0x00, 0x00, 0x00, 0xC8, 0x65, 0x80, 0x00, 0x00, 0x00, 0x76, 0x1A, 0x80, 0x00, 0x00,
0x00, 0x53, 0x86, 0x80, 0x00, 0x00, 0x00, 0x59, 0xE0, 0x40, 0x00, 0x00, 0x00, 0x4C, 0xFF, 0x60,
0x00, 0x00, 0x00, 0x2E, 0x7F, 0xE0, 0x00, 0x00, 0x00, 0x27, 0x9F, 0xF0, 0x00, 0x00, 0x00, 0x07,
0xCF, 0xF8, 0x00, 0x00, 0x00, 0x13, 0xE7, 0xF8, 0x00, 0x00, 0x00, 0x03, 0xE1, 0xFC, 0x00, 0x00,
0x00, 0x00, 0xE0, 0xFE, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x70, 0x0F,
0x80, 0x00, 0x00, 0x00, 0x30, 0x07, 0xC0, 0x00, 0x00, 0x00, 0x10, 0x00, 0xF0, 0x00, 0x00, 0x00,
0x08, 0x00, 0x30, 0x00, 0x00, 0x00, 0x04, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x03, 0x00, 0x03,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02,
};

const uint8_t rook_bitmap[] PROGMEM = {
0x00, // 00000000
0x55, // 01010101
0x7f, // 01111111
0x3e, // 00111110
0x3e, // 00111110
0x3e, // 00111110
0x3e, // 00111110
0x7f // 01111111
};
*/
void DisplaySetup::initialize(void)
{
    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.setFont(u8g2_font_helvB12_te);
    u8g2.setFontMode(0);
}

void DisplaySetup::splashScreen(const char *splashText)
{
    u8g2_uint_t offset = 32;
    u8g2_uint_t width;
    width = u8g2.getUTF8Width(splashText);
    u8g2_uint_t x;

    while (offset > 0)
    {
        u8g2.firstPage();
        do
        {
            x = offset;
            u8g2.drawUTF8(x, 38, splashText);
            x += width;
        } while (u8g2.nextPage());
        offset -= 1;
        delay(1);
    }
}

void DisplayOperation::displayu8g2(){
	u8g2.display();
}	

void DisplayOperation::clearDisplay(void)
{
    u8g2.clearDisplay();
}

void BoardOperation::emptyRectangle(int16_t startX, int16_t startY, int16_t endX, int16_t endY)
{
    u8g2.firstPage();
    do
    {
        u8g2.drawRFrame(startX, startY, endX - startX, endY - startY, 3);
    } while (u8g2.nextPage());
    delay(1);
}

void BoardOperation::fullRectangle(int16_t startX, int16_t startY, int16_t endX, int16_t endY)
{
    u8g2.firstPage();
    do
    {
        u8g2.drawRBox(startX, startY, endX - startX, endY - startY, 3);
    } while (u8g2.nextPage());
    delay(1);
}

void BoardOperation::displayBoard(
    const char board[][COL],
    const int16_t cursorXPosition,
    const int16_t cursorYPosition,
    const int8_t isCursorActive)
{
    int16_t rowHeight = HEIGHT / ROW;
    int16_t columnWidth = WIDTH / COL;
    int16_t rowSpacing = HEIGHT - rowHeight * ROW;
    int16_t colSpacing = WIDTH - columnWidth * COL;
    int16_t rowSpacingMargin = rowSpacing / 2;
    int16_t colSpacingMargin = colSpacing / 2;

    u8g2.firstPage();
    do
    {
        for (int i = 0; i < ROW; ++i)
        {
            for (int j = 0; j < COL; ++j)
            {
                if (isCursorActive)
                {
                    if (board[i][j] == FULL_CELL)
                    {
                        u8g2.drawRBox(colSpacingMargin + j * columnWidth,
                                      rowSpacingMargin + i * rowHeight,
                                      columnWidth,
                                      rowHeight,
                                      1);
                    }
                    else
                    {
                        u8g2.drawRFrame(colSpacingMargin + j * columnWidth,
                                        rowSpacingMargin + i * rowHeight,
                                        columnWidth,
                                        rowHeight,
                                        1);
                    }
                }
                else
                {
                    if (i != cursorXPosition || j != cursorYPosition)
                    {
                        if (board[i][j] == FULL_CELL)
                        {
                            u8g2.drawRBox(colSpacingMargin + j * columnWidth,
                                          rowSpacingMargin + i * rowHeight,
                                          columnWidth,
                                          rowHeight,
                                          1);
                        }
                        else
                        {
                            u8g2.drawRFrame(colSpacingMargin + j * columnWidth,
                                            rowSpacingMargin + i * rowHeight,
                                            columnWidth,
                                            rowHeight,
                                            1);
                        }
                    }
                    else
                    {
                        u8g2.drawRFrame(colSpacingMargin + j * columnWidth,
                                        rowSpacingMargin + i * rowHeight,
                                        columnWidth,
                                        rowHeight,
                                        1);
                    }
                }
            }
        }
    } while (u8g2.nextPage());
    delay(1);
}

void BoardOperation::goUp(char board[][COL], int16_t *cursorXPosition, int16_t *cursorYPosition)
{
	if(!(*cursorXPosition >= 0 && *cursorXPosition <= ROW - 1))
		return 0;

    direction = UP;


    if (*cursorXPosition == ROW - 1)
    {
		board[*cursorXPosition][*cursorYPosition] = EMPTY_CELL;
        board[--*cursorXPosition][*cursorYPosition] = FULL_CELL;
    }
    else if (*cursorXPosition > 0)
    {
        board[--*cursorXPosition][*cursorYPosition] = FULL_CELL;
    }
}

void BoardOperation::goDown(char board[][COL], int16_t *cursorXPosition, int16_t *cursorYPosition)
{
	if(!(*cursorXPosition >= 0 && *cursorXPosition <= ROW))
		return 0;

    direction = DOWN;
    if (*cursorXPosition < ROW - 1)
    {
        board[++*cursorXPosition][*cursorYPosition] = FULL_CELL;
    }
}

void BoardOperation::goRight(char board[][COL], int16_t *cursorXPosition, int16_t *cursorYPosition)
{
	if(!(*cursorYPosition < COL - 1))
		return 0;

    direction = RIGHT;
    if (*cursorXPosition == ROW - 1)
    {
        board[*cursorXPosition][*cursorYPosition] = EMPTY_CELL;
        board[*cursorXPosition][++*cursorYPosition] = FULL_CELL;
    }
    else if (*cursorYPosition < COL - 1)
    {
        board[*cursorXPosition][++*cursorYPosition] = FULL_CELL;
    }
}

void BoardOperation::goLeft(char board[][COL], int16_t *cursorXPosition, int16_t *cursorYPosition)
{
	if(!(*cursorYPosition > 0))
		return 0;

    direction = LEFT;
    if (*cursorXPosition == ROW - 1)
    {
        board[*cursorXPosition][*cursorYPosition] = EMPTY_CELL;
        board[*cursorXPosition][--*cursorYPosition] = FULL_CELL;
    }
    else if (*cursorYPosition > 0)
    {
        board[*cursorXPosition][--*cursorYPosition] = FULL_CELL;
    }
}

void BoardOperation::initializeBoard(char board[][COL])
{
    for (int i = 0; i < ROW; ++i)
    {
        for (int j = 0; j < COL; ++j)
        {
            board[i][j] = EMPTY_CELL;
        }
    }
	board[ROW-1][0] = FULL_CELL;
}

void BoardOperation::changeCursorState(int8_t *isCursorActive)
{
    if (*isCursorActive)
    {
        *isCursorActive = 0;
    }
    else
    {
        *isCursorActive = 1;
    }
}

void StateOperation::changeState(const char *stateName)
{
    u8g2.firstPage();
    do
    {
        u8g2.drawUTF8((128 - u8g2.getUTF8Width(stateName)) / 2, 38, stateName);
    } while (u8g2.nextPage());
    delay(1000);
}

void StateOperation::load(void)
{
    u8g2.drawPixel(64, 20);
    u8g2.display();
    delay(10);
    u8g2.drawPixel(66, 21);
    u8g2.display();
    delay(20);
    u8g2.drawPixel(68, 24);
    u8g2.display();
    delay(30);
    u8g2.drawPixel(68, 26);
    u8g2.display();
    delay(40);
    u8g2.drawPixel(67, 28);
    u8g2.display();
    delay(50);
    u8g2.drawPixel(64, 30);
    u8g2.display();
    delay(60);

    u8g2.drawPixel(62, 30);
    u8g2.display();
    delay(70);
    u8g2.drawPixel(59, 28);
    u8g2.display();
    delay(90);
    u8g2.drawPixel(58, 26);
    u8g2.display();
    delay(110);
    u8g2.drawPixel(58, 24);
    u8g2.display();
    delay(130);
    u8g2.drawPixel(59, 22);
    u8g2.display();
    delay(150);
    u8g2.drawPixel(62, 20);
    u8g2.display();
    delay(170);
}

void StateOperation::unload(void)
{
    u8g2.drawPixel(64, 20);
    u8g2.display();
    delay(10);
    u8g2.drawPixel(66, 21);
    u8g2.display();
    delay(20);
    u8g2.drawPixel(68, 24);
    u8g2.display();
    delay(30);
    u8g2.drawPixel(68, 26);
    u8g2.display();
    delay(40);
    u8g2.drawPixel(67, 28);
    u8g2.display();
    delay(50);
    u8g2.drawPixel(64, 30);
    u8g2.display();
    delay(60);

    u8g2.drawPixel(62, 30);
    u8g2.display();
    delay(70);
    u8g2.drawPixel(59, 28);
    u8g2.display();
    delay(90);
    u8g2.drawPixel(58, 26);
    u8g2.display();
    delay(110);
    u8g2.drawPixel(58, 24);
    u8g2.display();
    delay(130);
    u8g2.drawPixel(59, 22);
    u8g2.display();
    delay(150);
    u8g2.drawPixel(62, 20);
    u8g2.display();
    delay(170);
}

void Test::drawBoardTest(void)
{
    int loopCounter = 0;
    uint8_t isCursorActive = 0;
    int isSplashShown = 0;
    int x = 0;
    int y = 0;
    char board[10][10];

    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            board[i][j] = EMPTY_CELL;
        }
    }

    while (1)
    {
        board[x][y] = FULL_CELL;
        BoardOperation::displayBoard(board, x, y, isCursorActive);

        delay(10);

        if (loopCounter == 0)
        {
            for (int i = 0; i < 10; ++i)
            {
                BoardOperation::displayBoard(board, 0, 0, isCursorActive);
                BoardOperation::changeCursorState(&isCursorActive);
                delay(10);
            }
        }

        switch (loopCounter)
        {
        case 1:
            board[0][0] = FULL_CELL;
            for (int i = 0; i < 10; ++i)
            {
                BoardOperation::displayBoard(board, 0, 1, isCursorActive);
                BoardOperation::changeCursorState(&isCursorActive);
                delay(10);
            }
            break;
        case 2:
            board[0][1] = FULL_CELL;
            for (int i = 0; i < 10; ++i)
            {
                BoardOperation::displayBoard(board, 1, 1, isCursorActive);
                BoardOperation::changeCursorState(&isCursorActive);
                delay(10);
            }
            break;
        case 3:
            board[1][1] = FULL_CELL;
            for (int i = 0; i < 10; ++i)
            {
                BoardOperation::displayBoard(board, 1, 2, isCursorActive);
                BoardOperation::changeCursorState(&isCursorActive);
                delay(10);
            }
            break;
        case 4:
            board[1][2] = FULL_CELL;
            for (int i = 0; i < 10; ++i)
            {
                BoardOperation::displayBoard(board, 2, 2, isCursorActive);
                BoardOperation::changeCursorState(&isCursorActive);
                delay(10);
            }
            break;
        case 5:
            board[2][2] = FULL_CELL;
            for (int i = 0; i < 10; ++i)
            {
                BoardOperation::displayBoard(board, 2, 3, isCursorActive);
                BoardOperation::changeCursorState(&isCursorActive);
                delay(10);
            }
            break;
        case 6:
            board[2][3] = FULL_CELL;
            for (int i = 0; i < 10; ++i)
            {
                BoardOperation::displayBoard(board, 3, 3, isCursorActive);
                BoardOperation::changeCursorState(&isCursorActive);
                delay(10);
            }
            break;
        }

        if (loopCounter > 6)
        {
            for (int i = 0; i < 10; ++i)
            {
                BoardOperation::displayBoard(board, 3, 3, isCursorActive);
                BoardOperation::changeCursorState(&isCursorActive);
                delay(10);
            }
        }

        delay(10);
        ++loopCounter;
    }
}

void Test::runTests(void)
{
    DisplaySetup::splashScreen("Maze Game");
    delay(1000);
    BoardOperation::emptyRectangle(20, 15, 50, 37);
    delay(1000);
    BoardOperation::fullRectangle(20, 15, 50, 37);
    delay(1000);
    StateOperation::changeState("Loading");
    delay(1000);
    StateOperation::changeState("Exchanging");
    delay(1000);
    StateOperation::changeState("Starting");
    delay(1000);
    Test::drawBoardTest();
}
