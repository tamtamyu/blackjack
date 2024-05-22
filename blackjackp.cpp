#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <string>
using namespace std;

// カードを表す構造体
struct Card {
    string rank;
    string suit;
    int value;
};

// デッキを初期化する関数
vector<Card> createDeck() {
    vector<Card> deck;
    string suits[] = { " Heart ", "Diamond", " Club  ", " Spade " };
    string ranks[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
    int values[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11 };

    for (const string& suit : suits) {
        for (int i = 0; i < 13; ++i) {
            deck.push_back({ ranks[i], suit, values[i] });
        }
    }
    return deck;
}

// 手札の合計を計算する関数
int calculateHandValue(const vector<Card>& hand) {
    int value = 0;
    int aceCount = 0;

    for (const Card& card : hand) {
        value += card.value;
        if (card.rank == "A") {
            aceCount++;
        }
    }

    while (value > 21 && aceCount > 0) {
        value -= 10;
        aceCount--;
    }

    return value;
}

// 手札を表示する関数
void displayHand(const vector<Card>& hand) {
    for (const Card& card : hand) {
        cout << "   " << card.rank << " of " << card.suit << "   ";
    }
    cout << endl << "Total value: " << calculateHandValue(hand) << endl << endl;
}

// デッキからカードを引く関数
Card drawCard(vector<Card>& deck) {
    Card card = deck.back();
    deck.pop_back();
    return card;
}

//全体の勝敗を決定する関数
int win_lose_judge(int p) {
    if (p <= 0) {
        cout << "GameSet" << endl << "You Lose... because of 0 point" << endl;
        return 1;
    }
    else if (p >= 20) {
        cout << "GameSet" << endl << "You Win!!! with " << p << "point" << endl;
        return 1;
    }
    else {
        return 2;
    }
}

int main() {
    cout << "10$を20$以上にできたらWin！" << endl;
    cout << "勝てばBet額獲得！負ければ没収！" << endl << endl;
    int n = 1;//勝負の回数の初期値
    int point = 10;//初期のチップ

    while(1){
        cout << n << "回目の勝負！" << endl;
        int flg = win_lose_judge(point);
        if (flg == 1) {
            break;
        }
        string input;
        int bet_num;
        cout << "現在保持しているチップ：" << point << "$" << endl << "Bet金額を入力して下さい！" << endl;
        while (true) {
            cin >> input;

            // 入力を整数に変換
            try {
                size_t pos;
                bet_num = stoi(input, &pos);

                // posがinputの長さと一致しない場合、余分な文字が含まれている
                if (pos != input.length() || bet_num > point) {
                    throw invalid_argument("Invalid input");
                }
                break; // 正常な入力があった場合ループを抜ける
            }
            catch (exception& e) {
                cout << "所持しているチップで足りる金額を入力して下さい！" << endl;
                cout << "現在保持しているチップ：" << point << "$" << endl;
            }
        }

        srand(time(0));
        vector<Card> deck = createDeck();
        random_shuffle(deck.begin(), deck.end());

        vector<Card> playerHand;
        vector<Card> dealerHand;

        playerHand.push_back(drawCard(deck));
        playerHand.push_back(drawCard(deck));
        dealerHand.push_back(drawCard(deck));
        dealerHand.push_back(drawCard(deck));

        cout << "Dealer's hand: " << "   " << dealerHand[0].rank << " of " << dealerHand[0].suit << "   " << "   a hidden card   " << endl;
        cout << "Player's hand: ";
        displayHand(playerHand);

        char choice;
        int flg2 = 0;
        int flg3 = 0;
        while (true) {
            cout << endl << "Do you want to hit (h) or stand (s)? ";
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cin >> choice;
            cout << endl;
            if (choice == 'h') {
                playerHand.push_back(drawCard(deck));
                cout << "Player's hand: ";
                displayHand(playerHand);

                if (calculateHandValue(playerHand) > 21) {
                    cout << "Player busts! Dealer wins." << endl;
                    point -= bet_num;
                    n++;
                    flg3 += 1;
                    break;
                }
            }
            else if (choice == 's') {
                break;
            }
        }
        if (flg3 == 0) {
            cout << "Dealer's hand: ";
            displayHand(dealerHand);

            while (calculateHandValue(dealerHand) < 17) {
                dealerHand.push_back(drawCard(deck));
                cout << "Dealer's hand: ";
                displayHand(dealerHand);

                if (calculateHandValue(dealerHand) > 21) {
                    cout << "Dealer busts! Player wins." << endl;
                    point += bet_num;
                    n++;
                    flg2 += 1;
                }
            }

            if (flg2 == 0) {
                int playerTotal = calculateHandValue(playerHand);
                int dealerTotal = calculateHandValue(dealerHand);

                cout << endl << "Player's final hand value: " << playerTotal << endl << endl;
                cout << "Dealer's final hand value: " << dealerTotal << endl << endl;

                if (playerTotal > dealerTotal) {
                    cout << "Player wins!" << endl;
                    point += bet_num;
                    n++;
                }
                else if (playerTotal < dealerTotal) {
                    cout << "Dealer wins!" << endl;
                    point -= bet_num;
                    n++;
                }
                else {
                    cout << "It's a tie!" << endl;
                    n++;
                }
            }
        }
    }
    

    return 0;
}
