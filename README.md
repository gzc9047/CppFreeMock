CppFreeMock
===========

Based on gmock, can mock global function, member function, class static function without change source code.

[Here](https://onedrive.live.com/redir.aspx?cid=4c6fdc828365b80e&page=view&resid=4C6FDC828365B80E!28191&parId=4C6FDC828365B80E!28141&authkey=!AtpjZailG7DIcVg&Bpub=SDX.SkyDrive&Bsrc=Share) is the document and design notes host on OneNote.

Hello world:

```cpp
string func() {
    return "Non mocked.";
}

TEST(HelloWorld, First) {
    EXPECT_CALL(*MOCKER(func), MOCK_FUNCTION()).Times(Exactly(1))
        .WillOnce(Return("Hello world."));
    EXPECT_EQ("Hello world.", func());
}
```

[Here](https://github.com/gzc9047/cpp_non_virtual_mock) is the prototype when I try to solve this problem.