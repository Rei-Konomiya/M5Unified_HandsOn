#include <Arduino.h>
#include <M5Unified.h>

/*
  注意：
    表示にSpriteを使用しているため、
    表示を変更する際は M5Unified におけるSpriteの知識が必要になる場合があります。
*/

/*表示用スプライト*/
static M5GFX& lcd(M5.Display);
static LGFX_Sprite sprite(&lcd);

void setup(void) {
  auto cfg = M5.config();   // 必須
  M5.begin(cfg);            // 必須
  /*以下３つは表示用スプライトの設定*/
  sprite.setColorDepth(8);
  sprite.createSprite(lcd.width(), lcd.height());
  sprite.setTextSize(1.5);
}

void loop(void) {
  M5.update();    // タッチやボタンの操作状態の更新。これがないと値は更新されない

  lcd.startWrite();     // 表示処理を軽く早くするおまじない。endWrite()とセットで使う
  sprite.startWrite();  // 同上
  if (M5.Touch.isEnabled()) {
    auto t = M5.Touch.getDetail();  // タッチ操作に関する値の取得

    /*ここから非表示*/
    auto bMsec = t.base_msec; // 電源がついてからの時間(ms単位)、おそらく長押しなどの0.5秒の判定はこれを使っていると予想
    
    auto pr = t.prev; // 座標値のハード側の値を出したもの、値の中身は (prev_y * 65536) + prev_x で求められる
    auto prx = t.prev_x;  // x座標のハード値
    auto pry = t.prev_y;  // y座標のハード値
    
    auto st = t.state;    // 状態遷移をbit値で管理しているもの。is~やwas~関数で返すそれぞれの値を取得・保持している。使用することはおそらくない
    auto s = t.size;      // おそらく判定範囲。不明
    /*ここまで非表示*/

    auto p = t.isPressed();   // 押されているとき1
    auto r = t.isReleased();  // 離れているとき1
    auto h = t.isHolding();   // 0.5秒以上長押しされているとき1
    auto f = t.isFlicking();  // 触れてから0.5秒以内に移動したとき1
    auto d = t.isDragging();  // 長押し状態で移動したとき1

    auto wp = t.wasPressed();   // 触れた瞬間のみ1
    auto wr = t.wasReleased();  // 離れた瞬間のみ1
    auto wc = t.wasClicked();   // 0.5秒以内に指が移動せず離れたとき1
    auto wh = t.wasHold();      // 0.5秒以上長押しされて移動せず離れたとき1
    auto wfs = t.wasFlickStart(); // フリック開始時のみ1
    auto wf = t.wasFlicked();   // 0.5秒以内に移動して離れたとき1
    auto wds = t.wasDragStart();  //ドラッグ開始時のみ1
    auto wd = t.wasDragged();   // 長押し状態で移動して離れたとき1

    auto x = t.x;         // 現在触れているx座標
    auto y = t.y;         // 現在触れているy座標
    auto bx = t.base_x;   // 触れた瞬間のx座標
    auto by = t.base_y;   // 触れた瞬間のy座標

    auto dix = t.distanceX(); // 触れた瞬間と現在触れている座標のx距離
    auto diy = t.distanceY(); // 触れた瞬間と現在触れている座標のy距離
    auto dex = t.deltaX();    // 前回検知した座標と現在検知した座標のx距離 (=ドラッグ・フリックx速度)
    auto dey = t.deltaY();    // 前回検知した座標と現在検知した座標のy距離 (=ドラッグ・フリックy速度)

    //同一値のためメンバは非表示
    auto cCount = t.click_count;  // 同じ座標で連続クリックされた回数（ホールド・フリック・別座標でのクリック、0.5秒以上の操作なしで0に戻る）
    auto gcCount = t.getClickCount(); // click_countの値を返す関数。つまりclick_countと同じ

    auto id = t.id;   // 2点同時押し時に1、ただしy座標がほぼ端から端まで離れていないと1点としてカウントされる為実用化は難しい

    sprite.setCursor(0, 0);

    // 1=true, 0=false
    sprite.printf("isPressed = %d, isReleased = %d\n", p, r);
    sprite.printf("isHolding = %d\n", h);
    sprite.printf("isFlicking = %d, isDragging = %d\n", f, d);
    sprite.println();
    
    // 1=true, 0=false 一瞬しか 1 にならないので見逃し注意
    sprite.printf("wasPressed = %d, wasReleased = %d\n", wp, wr);
    sprite.printf("wasClicked = %d, wasHold = %d\n", wc, wh);
    sprite.printf("wasFlickStart = %d, wasFlicked = %d\n", wfs, wf);
    sprite.printf("wasDragStart = %d, wasDragged = %d\n", wds, wd);
    sprite.println();

    // 座標値表示
    sprite.printf("x = %3d, y = %3d\n", x, y);
    sprite.printf("base_x = %3d, base_y = %3d\n", bx, by);
    sprite.println();

    // 座標値表示  左または上に向かって動かすと負数になる
    sprite.printf("distanceX = %4d, distanceY = %4d\n", dix, diy);
    sprite.printf("deltaX = %3d, deltaY = %3d\n", dex, dey);
    sprite.println();

    // どちらも判定が難しいため、動作を確認するには何度も試す必要アリ
    sprite.printf("getClickCount = %2d\n", gcCount);
    sprite.printf("id = %d\n", id);
  }

  sprite.pushSprite(&lcd, 0, 0); // これを消すと画面描画がされなくなるため注意
  sprite.endWrite();  // おまじない
  lcd.endWrite();     // おまじない

  delay(1); // 更新間隔は1ミリ秒で設定
}