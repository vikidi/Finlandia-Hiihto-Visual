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
    std::vector<QPixmap*> obstaclePixmaps_;
    QGraphicsSimpleTextItem* progressView_;
    QTimer* timer_;
    /*!
     * \brief gametick updates locations of game objects.
     * It is called using single shot timer.
     */
    void gametick();
};

}

#endif // GAMESCENE_H
