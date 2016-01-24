

#include "RedCoreNamespace.h"
#include "RedTmlNamespace.h"

#include "RedTestTml.h"

namespace Red {
namespace Test {

using namespace Red::Core;
using namespace Red::TinyML;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int RedTestTml::RunUnitTest(void)
{

    if (RedTestTml::TestOne().IsFail())       return 0;
    if (RedTestTml::TestTwo().IsFail())       return 0;
    if (RedTestTml::TestIterators().IsFail()) return 0;
    if (RedTestTml::TestTreeEdit().IsFail())  return 0;

    return 1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedResult RedTestTml::TestOne(void)
{
    RedString path  = "/tmp/Test.tml";
    RedString path2 = "/tmp/Test2.tml";

    // Create small Tml Structure
    RedTmlNode x("name");
    RedTmlNode* y = x.CreateChildNode("childnode");
    y->CreateChildLeaf("xyzname", "xyzdata");

    RedBufferOutput oB;
    RedTmlAction::SerialiseTinyML(oB, &x, eDenseContent);
    oB.WriteNewLine();
    RedIOHandler::OutputBufferToFile(path, oB);

    RedBufferInput iB;
    RedIOHandler::InputBufferFromFile(path, iB);
    RedTmlElement* newX = RedTmlAction::ParseTinyML(iB);

    if (!newX)
        return kResultFail;

    RedBufferOutput oB2;
    RedTmlAction::SerialiseTinyML(oB2, newX, eLinedIndentedContent);
    oB2.WriteNewLine();
    RedIOHandler::OutputBufferToFile(path2, oB2);

    return kResultSuccess;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedResult RedTestTml::TestTwo(void)
{
    {
        RedString pathFail  = "/tmp/TestNonExistentFile.tml";
        RedTmlElement* newTmlElement = REDNULL;

        // Check for a non-existant file. Has to return fail
        RedResult resultOne = RedTmlAction::CreateTmlFromFile(pathFail, &newTmlElement);
        if (resultOne != kResultFail)
            return kResultFail;
    }

    RedString pathSave1  = "/tmp/TestTwo1.tml";
    RedString pathSave2  = "/tmp/TestTwo2.tml";
    {
        // Create small Tml Structure
        RedTmlNode oTop("tmltree");
        RedTmlNode* y = oTop.CreateChildNode("childnode");
        y->CreateChildLeaf("xyzname1", "xyzdata1");
        y->CreateChildLeaf("xyzname2", "xyzdata2");

        if (RedTmlAction::CreateFileFromTml(&oTop, pathSave1, eLinedIndentedContent) != kResultSuccess)
            return kResultFail;
    }

    {
        RedTmlElement* newTmlElement = REDNULL;
        if (RedTmlAction::CreateTmlFromFile(pathSave1, &newTmlElement) != kResultSuccess)
            return kResultFail;

        if (RedTmlAction::CreateFileFromTml(newTmlElement, pathSave2, eLinedIndentedContent) != kResultSuccess)
            return kResultFail;
    }
    return kResultSuccess;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedResult RedTestTml::TestIterators(void)
{
    {
        // Create small Tml Structure
        RedTmlNode x("topnode");
        RedTmlNode* y = x.CreateChildNode("contentnode");
        y->CreateChildLeaf("name1", "data1");
        y->CreateChildLeaf("name2", "data2");
        y->CreateChildLeaf("name3", "data3");
        y->CreateChildLeaf("name4", "data4");
        y->CreateChildLeaf("name5", "data5");

        TinyML::RedTmlNode::TmlNodeListItType yIt = y->NodeIterator();

        int count = 0;
        yIt.First();
        while (!yIt.IsDone())
        {
            RedTmlElement* pCurrElem = yIt.CurrentItem();

            if (pCurrElem != REDNULL)
                count++;
            yIt.Next();
        }

        if (count != 5)
            return kResultFail;
    }

    {
        // Create small Tml Structure
        RedTmlNode x("topnode");
        RedTmlNode* y = x.CreateChildNode("contentnode");
        y->CreateChildLeaf("name1", "data1");
        y->CreateChildLeaf("name2", "data2");
        y->CreateChildLeaf("name3", "data3");
        y->CreateChildLeaf("name3", "data4");
        y->CreateChildLeaf("name3", "data5");

        // Child search checks
        if (!RedTmlAction::ChildLeafExists(*y, RedString("name2")))
            return kResultFail;
        if (RedTmlAction::NumberOfNamedChildLeaves(*y, RedString("name3")) != 3)
            return kResultFail;

        // Tree count checks
        if (RedTmlAction::TreeElementCount(x) != 7)
            return kResultFail;
        if (RedTmlAction::TreeElementCount(*y) != 6)
            return kResultFail;
        if (x.NodeElementCount() != 1)
            return kResultFail;
        if (y->NodeElementCount() != 5)
            return kResultFail;

        int count = 0;
        TinyML::RedTmlNode::TmlNodeListItType yIt = y->NodeIterator();
        yIt.First();
        while (!yIt.IsDone())
        {
            RedTmlElement* pCurrElem = yIt.CurrentItem();

            if (pCurrElem != REDNULL)
                count++;
            yIt.Next();
        }

        if (count != 5)
            return kResultFail;
    }

    return kResultSuccess;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

RedResult RedTestTml::TestTreeEdit(void)
{
    {
        // Create small Tml Structure
        RedTmlNode x("topnode");
        RedTmlNode* y = x.CreateChildNode("contentnode");
        y->CreateChildLeaf("name1", "data1");
        y->CreateChildLeaf("name2", "data2");
        y->CreateChildLeaf("name3", "data3");
        y->CreateChildLeaf("name4", "data4");
        y->CreateChildLeaf("name5", "data5");

        RedTmlAction::SetChildLeaf(*y, RedString("name2"), RedString("NEWDATA2"));

        RedString resultName;
        if (RedTmlAction::ChildLeafNameForData(*y, RedString("NEWDATA2"), resultName) != kResultSuccess)
            return kResultFail;

        if (resultName != RedString("name2"))
            return kResultFail;

    }
    return kResultSuccess;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

} // Test
} // Red