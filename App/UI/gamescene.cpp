#include "gamescene.h"

InternetExplorers::GameScene::GameScene(bool gamemode, QObject* parent) :
    QGraphicsScene(parent),
    gameMode_(gamemode),
    running_(false),
    ground_(new QGraphicsRectItem(0,30,600,2)),
    skiAnimation_(new QMovie("://GraphicalResources/skier.gif")),
    skiLabel_(new QLabel),
    skierHeight_(0),
    skierSpeed_(0),
    timer_(nullptr)
{
    // Add images of different obstacles to vector
    // Currently only one object is used
    obstaclePixmaps_.push_back(new QPixmap(QPixmap::fromImage(QImage("://GraphicalResources/skier.gif"))));

    // Initial scene size
    this->setSceneRect(0,0,600,100);
    this->setBackgroundBrush(Qt::white);

    // Add ground
    ground_->setBrush(Qt::black);
    this->addItem(ground_);
    ground_->setY(50);

    // Create skier
    skiLabel_->setAttribute(Qt::WA_TranslucentBackground);
    skiLabel_->setMovie(skiAnimation_);
    skiAnimation_->setSpeed(100);
    skiAnimation_->jumpToFrame(0);
    skier_ = this->addWidget(skiLabel_);
    skier_->setY(ground_->y());
    skier_->setX(0);

    // Add progress bar as numeric value
    progressView_ = this->addSimpleText(QString::number(0));
    progressView_->setX(265);
    progressView_->setY(ground_->y() + 30);
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
        skiAnimation_->setSpeed(200);
        srand(4); // Chosen with fair dice roll
        startNewGame();
        auto infotext = this->addSimpleText(QString("Buttons: Up/Down"));
        infotext->setX(480);
        infotext->setY(-10);
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
    for(auto i : obstacles_)
    {
        this->removeItem(i);
        delete i;
    }
    obstacles_.clear();
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
        // Check if first obstacle has moved out of sight
        if(obstacles_.size() != 0)
        {
            if(obstacles_.at(0)->x() < -30)
            {
                this->removeItem(obstacles_.at(0));
                delete obstacles_.at(0);
                obstacles_.erase(obstacles_.begin());
            }
        }

        // Update obstacle locations
        for(auto obstacle : obstacles_)
        {
            obstacle->setX(obstacle->x()-4);
        }

        // Try to spawn more obstacles
        spawnObstacle();

        // Update skier height
        skierHeight_ += static_cast<int>(skierSpeed_);
        skierSpeed_ -= -0.7;
        if (skierHeight_ >= ground_->y())
        {
            skierSpeed_ = 0;
            skierHeight_ = ground_->y();
        }
        skier_->setY(skierHeight_);

        // Check for collisions
        for(auto item : skier_->collidingItems())
        {
            if(item != ground_)
            { // Game over
                running_ = false;
                return;
            }
        }

        // Restart timer
        timer_->start();
    }
}

void InternetExplorers::GameScene::startNewGame()
{
    for(auto obstacle : obstacles_)
    { // Remove possible old obstacles
        this->removeItem(obstacle);
        delete obstacle;
    }
    obstacles_.clear();
    skierHeight_ = ground_->y();
    skierSpeed_ = 0;
    skier_->setY(ground_->y());
    running_ = true;
    timer_->stop();
}

void InternetExplorers::GameScene::spawnObstacle()
{
    if(obstacles_.size() < 3)
    { // There is space for new obstacle
        if(obstacles_.size() != 0)
        { // There are already obstacles. Check if it just spawned
            if(obstacles_.back()->x() < 450)
            { // Old obstacles are not too close
                skipDistanceCheck:
                if(rand()%100 < 2)
                { // Somewhat 2% change of spawning new obstacle
                    auto obstacle = new QGraphicsPixmapItem(*obstaclePixmaps_.at(0));
                    this->addItem(obstacle);
                    obstacle->setY(ground_->y());
                    obstacle->setX(600);
                    obstacles_.push_back(obstacle);
                }
            }
        } else
        {
            // Check for previusly spawned obstacle can not be done
            goto skipDistanceCheck;
        }
    }
}

void InternetExplorers::GameScene::keyPressEvent(QKeyEvent *event)
{
    // Do nothing if this is not a game
    if(!gameMode_) return;

    switch (event->key()) {
    case Qt::Key_Space:
        space:
        if( skierHeight_ == ground_->y())
        {
            skierSpeed_ = -10;
        }
        if(running_)
        {
            timer_->start();
            skiAnimation_->start();
        } else
        {
            startNewGame();

        }
        break;
    case Qt::Key_Up:
        // Do same as pressing space
        goto space;
    case Qt::Key_Down:
        // Go down faster
        skierSpeed_ = 10;
        break;
    case Qt::Key_Left:
        break;
    case Qt::Key_Right:
        break;
    case Qt::Key_P:
        // Pause or unpause
        if(running_)
        {
            timer_->stop();
            running_ = false;
        } else
        {
            timer_->start();
            running_ = true;
        }
        break;
    default:
        break;
    }
}
