#ifndef BOARDPRINTERCONSOLE_H
#define BOARDPRINTERCONSOLE_H


class IAbaloneBoard;

class BoardPrinterConsole
{
public:
    virtual void Print(const IAbaloneBoard* board);
};

#endif // BOARDPRINTERCONSOLE_H
