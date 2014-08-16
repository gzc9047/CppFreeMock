CppFreeMock
===========

Based on gmock, can mock global function, member function, class static function without change source code.

[Here](https://onedrive.live.com/redir?resid=4C6FDC828365B80E!28191&authkey=!AK5mz05lh29D2eQ&ithint=onenote%2c)(writing) is the document and design notes host on OneNote.

Hello world:

```
string func() {
    return "Non mocked.";
}

TEST(HelloWorld, First) {
    CreateMocker(mocker, func);
    EXPECT_CALL(*mocker, MockFunction()).Times(Exactly(1))
        .WillOnce(Return("Hello world."));
    EXPECT_EQ("Hello world.", func());
}
```
