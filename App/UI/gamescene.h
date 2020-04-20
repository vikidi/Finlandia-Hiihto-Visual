#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QtCore/QObject>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsProxyWidget>
#include <QMovie>
#include <QLabel>
#include <QTimer>
#include <vector>

namespace InternetExplorers {

/*!
 * \brief
 * This class provides the jumping game.
 */
class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    /*!
     * \brief GameScene
     * \param gamemode True if this instance has game.
     * False is default and makes this a custom progress bar.
     * \param parent for Qt features
     */
    explicit GameScene(bool gamemode = false, QObject* parent = nullptr);
    ~GameScene();

public slots:
    /*!
     * \brief updateProgress updates skier location
     * and numeric value of progress
     * \param progress 0-100
     */
    void updateProgress(const int progress);

private:
    /*!
     * \brief gameMode_ True is game, false is progressbar
     */
    const bool gameMode_;
    /*!
     * \brief running_ is the game running or at pause
     */
    bool running_;

    // Graphics items
    QGraphicsRectItem* ground_;
    QMovie* skiAnimation_;
    QLabel* skiLabel_;
    QGraphicsProxyWidget* skier_;
    QGraphicsSimpleTextItem* progressView_;

    // Game related parts
    int skierHeight_;
    float skierSpeed_;
    std::vector<QPixmap*> obstaclePixmaps_;
    std::vector<QGraphicsPixmapItem*> obstacles_;
    QTimer* timer_;
    /*!
     * \brief gametick updates locations of game objects.
     * It is called using single shot timer.
     */
    void gametick();
    /*!
     * \brief startNewGame clears extra objects from scene
     * and moves player to starting position
     */
    void startNewGame();
    /*!
     * \brief spawnObstacle sometimes spawns new obstacle
     * if there is space available
     */
    void spawnObstacle();
    void keyPressEvent(QKeyEvent *event) override;
};

}

#endif // GAMESCENE_H
