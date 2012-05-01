
21歳頃に書いたのコードがいまだにネットに転がってたので思い出として保存。これ公開した頃にはプログラミングはほとんどしなくなってて、ビデオ屋のバイトでえっちなビデオを観光客に売っていたようなwんまぁ、それにしてもコードとコメントと拡張子が我ながら微笑ましいw

振り返ってみると、この当時のMacアプリの開発ってイベント駆動型の開発な上に、ウィンドウとかスクロールバーなんてGUIリソースも外部ファイルで（ResEditで）作って置いておいて必要な時に呼び出して描画処理したり、スクロールバーの位置や内容も自分で調整したり、GUIパーツの再描画の呼び出しも自分でやんなきゃな世界だったんだよなぁ。

そんな風にパソコン＋プログラミングを始めた時からそんな世界に慣れ親しんでたから、今時のWebでやるリソース面でのCSS SpriteとかNodeJSの非同期処理なんてのがOS9に近いくらいの印象だったり、meteorはOS10.0な感じ。デスクトップアプリとの違いはインターフェイスはローカルでロジックがローカルかリモートにあるかの違いだったり。もちろん多数のユーザーが同時に使うというのが大きな違いではあるけど。

なんだかんだでWebアプリの動きはゆっくりとデスクトップアプリの動きに近づいてくんだろなと思う今日この頃。

---

> mtFloatingWindowLibについて

> 早速mtFloatingWindowLibについての説明なんですが、
各関数の機能はヘッダファイルに記載しているので、なにを説明したらいいのか・・・。

> とりあえずmtFloatingWindowLibを使用する際の注意点を書いておきます。

> フローティングウィンドウのRefConの設定と取得には
	
>	//FloatWindowのRefConを設定する
	void mtSetFWRefCon( WindowPtr theFloatWindow, long data );
	//FloatWindowのRefConを取得する
	long mtGetFWRefCon( WindowPtr theFloatWindow );

> を使用して下さい。これを使用しないと他の関数が正常に動作しません。
※フローティングウィンドウにGetWRefConやSetWRefConを使用すると
　フローティングウィンドウの実現のためのデータが消えてしまいます）


> それでは、次は変更点の説明です。

> ファイル"mtFloatWindowScrl.c"内で

> if( (*theScrl) != nil )
{
	・・・・・・
	・・・・・
}

> 上記の様に記述していた部分がありました、これは私の原因は分りませんが
なぜか"無効なポインタ"と分らなかったのです、そして私が未熟者なので上記の
様に書いてしまいました御容赦下さい。そして、いつの間にか正状に機能する様に
なっていましたので下記のように訂正しておきました。
また、アピアランス1.0以降のものではちょっとした問題が発生する様なので、
そこ辺りも修正しておきました。

> if( theScrl != nil )
{
	・・・・・・
	・・・・・
}

> 次は下記の関数についてです。

> //ウィンドウ・ズームボックス処理
void priWindowZoom( WindowPtr theWindow, short partCode, Point clickPt );

> 上記の関数のせいで下記の関数に悪影響を及ぼしておりました、そこを直したのですが
一度ウィンドウをズームさせてからもう一度ズームボックスをクリックしても前の状態に
戻らないといった問題があります。

> //ウィンドウ・サイズ処理
void priWindowGrow( WindowPtr theWindow, Point clickPt,
					GetZoomGrowSizeData zoom_growSize );



> まあこの程度でしょうか。
ソースコードも付属してますので、自分でフローティングウィンドウを作る際に
参考にして頂ければと思っています。
※私は（コンピュータ歴 1年程度・プログラミング歴 1年程度 の）まだまだ初心者なので
　ソースコードが読みにくいとかは勘弁して下さい。(;^_^A ｱｾｱｾ･･･

> また、ご意見・ご感想・こんな風に書いた方がいいよ・・・等々ありましたらメールをください。
