#include <QCoreApplication>
#include <QtTest/QTest>
#include "TestMessage.h"
#include "TestFile.h"

int main(int c, char **v)
{
  QCoreApplication app(c,v);

  TestMessage tm;
  QTest::qExec(&tm);

  TestFile tf;
  QTest::qExec(&tf);
}
