#include "gamescene.h"

InternetExplorers::GameScene::GameScene(bool gamemode, QObject* parent) :
    QGraphicsScene(parent),
    gameMode_(gamemode),
    running_(false),
    ground_(new QGraphicsRectItem(0,30,600,2)),
    skiAnimation_(new QMovie("://GraphicalResources/skier.gif")),
    skiLabel_(new QLabel),
    timer_(nullptr)
{
    // Add images of different obstacles to vector
//    obstaclePixmaps_.push_back(new QPixmap(QPixmap::fromImage(QImage("://GraphicalResources/skier.gif"))));

    // Initial scene size
    this->setSceneRect(0,0,600,100);
    this->setBackgroundBrush(Qt::white);

    // Add ground
    ground_->setBrush(Qt::black);
    this->addItem(ground_);

    // Create skier
    skiLabel_->setAttribute(Qt::WA_TranslucentBackground);
    skiLabel_->setMovie(skiAnimation_);
    skiAnimation_->setSpeed(100);
    skiAnimation_->jumpToFrame(0);
    skier_ = this->addWidget(skiLabel_);
    skier_->setY(0);
    skier_->setX(0);

    // Add progress bar as numeric value
    progressView_ = this->addSimpleText(QString::number(0));
    progressView_->setX(265);
    progressView_->setY(30);
    progressView_->setBrush(Qt::black);
    progressView_->setOpacity(1);
    progressView_->setScale(2);

    if(gameMode_)
    {
        // Init timer that creates game ticks
        timer_ = new QTimer;
        timer_->setSingleShot(true);
        timer_->setInterval(1000/60); // ~60 FPS
        connect(timer_, &QTimer::timeout, this, &GameScene::gametick);
    } else
    {
        skiAnimation_->start();
    }
}

InternetExplorers::GameScene::~GameScene()
{
    delete ground_;
    delete skiLabel_;
    delete skiAnimation_;
    for(auto i : obstaclePixmaps_)
    {
        delete i;
    }
    if(gameMode_) delete timer_;
}

void InternetExplorers::GameScene::updateProgress(const int progress)
{
    progressView_->setText(QString::number(progress));
    if(!gameMode_)
    {
        skier_->setX(this->sceneRect().size().width()*(static_cast<float>(progress)/100.0)-15);
    }
}

void InternetExplorers::GameScene::gametick()
{
    if(running_)
    {
        // Restart timer
        timer_->start();
    }
}
