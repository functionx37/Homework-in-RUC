import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Deque;
import java.util.List;
import java.util.Random;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class PlayBridgeCards {
    private static final int PLAYER_COUNT = 4;
    private static final int CARDS_PER_PLAYER = 13;

    public static void main(String[] args) throws InterruptedException {
        Dealer dealer = new Dealer();
        List<Deque<Card>> hands = dealer.deal(PLAYER_COUNT, CARDS_PER_PLAYER);
        CardGame game = new CardGame(hands);
        game.startGame();
    }

    private static final class CardGame {
        private final ReentrantLock lock = new ReentrantLock();
        private final Condition[] playerConditions = new Condition[PLAYER_COUNT];
        private final Condition roundFinished = lock.newCondition();
        private final List<Deque<Card>> hands;
        private final Card[] cardsOnTable = new Card[PLAYER_COUNT];
        private final CountDownLatch readyLatch = new CountDownLatch(PLAYER_COUNT);

        private int currentTurn = -1;
        private int roundLeader = 0;
        private int playedCount = 0;
        private boolean roundDone = false;
        private boolean gameOver = false;

        private CardGame(List<Deque<Card>> hands) {
            this.hands = hands;
            for (int i = 0; i < PLAYER_COUNT; i++) {
                playerConditions[i] = lock.newCondition();
            }
        }

        public void startGame() throws InterruptedException {
            Thread[] players = new Thread[PLAYER_COUNT];
            for (int i = 0; i < PLAYER_COUNT; i++) {
                players[i] = new Thread(new Player(i));
                players[i].start();
            }

            readyLatch.await();

            for (int round = 0; round < CARDS_PER_PLAYER; round++) {
                lock.lock();
                try {
                    playedCount = 0;
                    roundDone = false;
                    currentTurn = roundLeader;
                    playerConditions[currentTurn].signal();

                    while (!roundDone) {
                        roundFinished.await();
                    }

                    int winner = determineWinner(roundLeader);
                    System.out.println("Winner: " + winner + "  round:" + round);

                    roundLeader = winner;
                    Arrays.fill(cardsOnTable, null);

                    if (round == CARDS_PER_PLAYER - 1) {
                        gameOver = true;
                        signalAllPlayers();
                    }
                } finally {
                    lock.unlock();
                }
            }

            for (Thread player : players) {
                player.join();
            }
        }

        private int determineWinner(int leader) {
            int winner = leader;
            Card bestCard = cardsOnTable[leader];
            for (int offset = 1; offset < PLAYER_COUNT; offset++) {
                int playerId = (leader + offset) % PLAYER_COUNT;
                Card currentCard = cardsOnTable[playerId];
                if (currentCard.getNumber() > bestCard.getNumber()) {
                    bestCard = currentCard;
                    winner = playerId;
                }
            }
            return winner;
        }

        private void signalAllPlayers() {
            for (Condition condition : playerConditions) {
                condition.signalAll();
            }
        }

        private final class Player implements Runnable {
            private final int playerId;

            private Player(int playerId) {
                this.playerId = playerId;
            }

            @Override
            public void run() {
                readyLatch.countDown();
                while (true) {
                    lock.lock();
                    try {
                        while (!gameOver && currentTurn != playerId) {
                            playerConditions[playerId].await();
                        }

                        if (gameOver) {
                            return;
                        }

                        Card card = hands.get(playerId).removeFirst();
                        cardsOnTable[playerId] = card;
                        playedCount++;
                        System.out.println(Thread.currentThread().getName() + ": " + card);

                        if (playedCount == PLAYER_COUNT) {
                            currentTurn = -1;
                            roundDone = true;
                            roundFinished.signal();
                        } else {
                            currentTurn = (playerId + 1) % PLAYER_COUNT;
                            playerConditions[currentTurn].signal();
                        }
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                        return;
                    } finally {
                        lock.unlock();
                    }
                }
            }
        }
    }

    private static final class Card {
        private final char suit;
        private final int number;

        private Card(char suit, int number) {
            this.suit = suit;
            this.number = number;
        }

        public int getNumber() {
            return number;
        }

        @Override
        public String toString() {
            return String.valueOf(suit) + number;
        }
    }

    private static final class Dealer {
        private final List<Card> deck = new ArrayList<>(52);

        private Dealer() {
            char[] suits = {'C', 'D', 'H', 'S'};
            for (char suit : suits) {
                for (int number = 1; number <= 13; number++) {
                    deck.add(new Card(suit, number));
                }
            }
        }

        public List<Deque<Card>> deal(int playerCount, int cardsPerPlayer) {
            shuffle();
            List<Deque<Card>> hands = new ArrayList<>(playerCount);
            for (int i = 0; i < playerCount; i++) {
                hands.add(new ArrayDeque<>());
            }

            int cardIndex = 0;
            for (int round = 0; round < cardsPerPlayer; round++) {
                for (int player = 0; player < playerCount; player++) {
                    hands.get(player).addLast(deck.get(cardIndex++));
                }
            }
            return hands;
        }

        private void shuffle() {
            System.out.println("shuffling...");
            Random random = new Random();
            for (int i = deck.size() - 1; i > 0; i--) {
                int j = random.nextInt(i + 1);
                Card temp = deck.get(i);
                deck.set(i, deck.get(j));
                deck.set(j, temp);
            }
        }
    }
}
