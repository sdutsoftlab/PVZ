/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "CommonClass.h"
#include "LessonX.h"

//#define random(a,b) (rand()%(b-a)+a)	//c ���� [a, b) �����
////////////////////////////////////////////////////////////////////////////////
//
//
CGameMain		g_GameMain;
bool CGameMain::lianliankan_init = false;
extern int total_zombie;
extern int bowling_counter;
extern long double game_start;
//==============================================================================
//
// ����ĳ�������Ϊ��GameMainLoop����Ϊ��ѭ��������������ÿ֡ˢ����Ļͼ��֮�󣬶��ᱻ����һ�Ρ�

//==============================================================================
//
// ���캯��
CGameMain::CGameMain() :
	m_iGameState(1),
	timer(0),
	zombie_timer(0),
	sun_count(100),
	sun_num(new CTextSprite("SunCount")),
	game_map(new CSprite("background")),
	zombie_count(0),
	progress_bar(new Assist("ProgressBar")),
	progress_flag(new Assist("Flag")),
	progress_head(new Assist("ZombieHead")),

	// ģ���ʼ��
	t_ord_zombie(new OrdinaryZombie("OrdinaryZombie")),
	t_bar_zombie(new BarricadeZombie("BarricadeZombie")),
	t_buc_zombie(new BucketheadZombie("BucketheadZombie")),
	t_new_zombie(new NewspaperZombie("NewspaperZombie")),
	t_fot_zombie(new FootballZombie("FootballZombie")),
	t_pea_shooter(new PeaShooter("PeaShooter", nullptr)),
	t_pea(new Pea("Pea")),
	t_range(new Range("AttackRange")),
	t_sun(new Sun("Sun", 25)),
	t_sunflower(new Sunflower("Sunflower", nullptr, 0)),
	t_boom(new Boom("Boom")),
	t_cherry_bomb(new CherryBomb("CherryBomb", nullptr, 0)),
	t_wall_nut(new WallNut("WallNut")),
	t_potato_mine(new PotatoMine("PotatoMine", nullptr, 0)),
	t_jalapeno(new Jalapeno("Jalapeno", nullptr, 0)),
	t_three_peater(new ThreePeater("ThreePeater", std::vector<Pea*>(3))),

	// С����ʼ��
	tool_car(new Car("Car")),
	tool_shovel(new Shovel("Shovel")),
	// ����ʼ��
	pea_shooter_card(new PeaShooterCard("PeaShooterCard")), // ���ֶ�����map
	sunflower_card(new SunflowerCard("SunflowerCard")),
	cherry_bomb_card(new CherryBombCard("CherryBombCard")),
	potato_mine_card(new PotatoMineCard("PotatoMineCard")),
	wall_nut_card(new WallNutCard("WallNutCard")),
	jalapeno_card(new JalapenoCard("JalapenoCard")),
	three_peater_card(new ThreePeaterCard("ThreePeaterCard"))
{
	name_to_sprite[pea_shooter_card->GetName()] = pea_shooter_card;
	vec_card.push_back(pea_shooter_card);

	name_to_sprite[sunflower_card->GetName()] = sunflower_card;
	vec_card.push_back(sunflower_card);

	name_to_sprite[cherry_bomb_card->GetName()] = cherry_bomb_card;
	vec_card.push_back(cherry_bomb_card);

	name_to_sprite[wall_nut_card->GetName()] = wall_nut_card;
	vec_card.push_back(wall_nut_card);

	name_to_sprite[potato_mine_card->GetName()] = potato_mine_card;
	vec_card.push_back(potato_mine_card);

	name_to_sprite[jalapeno_card->GetName()] = jalapeno_card;
	vec_card.push_back(jalapeno_card);

	name_to_sprite[three_peater_card->GetName()] = three_peater_card;
	vec_card.push_back(three_peater_card);

	name_to_sprite[tool_shovel->GetName()] = tool_shovel;
	name_to_sprite[progress_bar->GetName()] = progress_bar;
	name_to_sprite[progress_flag->GetName()] = progress_flag;
	name_to_sprite[progress_head->GetName()] = progress_head;
}
//==============================================================================
//
// ��������
CGameMain::~CGameMain()
{
}

//==============================================================================
//
// ��Ϸ��ѭ�����˺���������ͣ�ĵ��ã�����ÿˢ��һ����Ļ���˺�����������һ��
// ���Դ�����Ϸ�Ŀ�ʼ�������С������ȸ���״̬. 
// ��������fDeltaTime : �ϴε��ñ��������˴ε��ñ�������ʱ��������λ����
void CGameMain::GameMainLoop(float	fDeltaTime)
{
	switch (GetGameState())
	{
		// ��ʼ����Ϸ�������һ���������
	case 1:
	{
		GameInit();
		SetGameState(2); // ��ʼ��֮�󣬽���Ϸ״̬����Ϊ������
	}
	break;

	// ��Ϸ�����У����������Ϸ�߼�
	case 2:
	{
		// TODO �޸Ĵ˴���Ϸѭ�������������ȷ��Ϸ�߼�
		if (true)
		{
			GameRun(fDeltaTime);
		}
		else // ��Ϸ������������Ϸ���㺯����������Ϸ״̬�޸�Ϊ����״̬
		{
			SetGameState(0);
			GameEnd();
		}
	}
	break;

	// ��Ϸ����/�ȴ����ո����ʼ
	case 0:
	default:
		break;
	};
}
//=============================================================================
//
// ÿ�ֿ�ʼǰ���г�ʼ���������һ���������
void CGameMain::GameInit()
{
	// ��������
	SuperSound::sendASoundCommand("open-menu");
	SuperSound::sendASoundCommand("play-menu");

	///*welcome.t2d
	if (map_id == MapType::WelcomeType) {
		CSystem::LoadMap("welcome.t2d");
		CSprite title("Title");
		title.SpriteMoveTo(-0.909, -27.080, 18, true);
		CSprite load("load");
		load.SetSpriteAngularVelocity(80);
		load.SpriteMoveTo(0.75 + 41.5 / 2 - 6, 30.875 - 11.75 / 2 + 2.7, 10, true);
		load.SetSpriteLifeTime(4);
	}
}
//=============================================================================
//
// ÿ����Ϸ������
void CGameMain::GameRun(float fDeltaTime)
{
	CSprite welcome("welcome");
	welcome.SpriteMoveTo(-28.883, -23.750, 18, true);

	switch (map_id) {
	case MapType::AdventureType:
		load_adventure_level(adventure_level_id, fDeltaTime);
		break;
	case MapType::BowlingType:
		load_bowling(fDeltaTime);
		break;
	case MapType::LianLianKanType:
		load_lianliankan(fDeltaTime);
		break;
	default:
		break;
	}

}
//=============================================================================
//
// ������Ϸ����
void CGameMain::GameEnd()
{
}

// �þ�������ӳ�侫�����
PvZSprite* CGameMain::get_sprite_by_name(const std::string& sprite_name) {
	if (name_to_sprite.count(sprite_name)) {
		return name_to_sprite.at(sprite_name);
	}
	return nullptr;
}
Car* CGameMain::create_car(float x, float y) {
	Car* car = new Car(CSystem::MakeSpriteName("Car", vec_car.size()));
	vec_car.push_back(car);
	name_to_sprite[car->GetName()] = car;

	car->CloneSprite(tool_car->GetName());
	car->SetSpritePosition(x, y);
	return car;
}


Zombie* CGameMain::create_ord_zombie(int y) {
	float y_slot[5] = { -17, -5, 9, 20, 32 };
	OrdinaryZombie* zombie = new OrdinaryZombie(CSystem::MakeSpriteName(t_ord_zombie->GetName(), vec_ord_zombie.size()));
	vec_ord_zombie.push_back(zombie);
	name_to_sprite[zombie->GetName()] = zombie;
	zombie->CloneSprite(t_ord_zombie->GetName());
	zombie->set_exist(true);
	zombie->SetSpritePosition(CSystem::GetScreenRight(), y_slot[y] - zombie->GetSpriteHeight() / 2);
	zombie->move();

	return zombie;
}
//BarricadeZombie
Zombie* CGameMain::create_bar_zombie(int y) {
	float y_slot[5] = { -17, -5, 9, 20, 32 };
	BarricadeZombie* zombie = new BarricadeZombie(CSystem::MakeSpriteName(t_bar_zombie->GetName(), vec_bar_zombie.size()));
	vec_bar_zombie.push_back(zombie);
	name_to_sprite[zombie->GetName()] = zombie;
	zombie->CloneSprite(t_bar_zombie->GetName());
	zombie->set_exist(true);
	int d = CSystem::RandomRange(10, 20);
	zombie->SetSpritePosition(CSystem::GetScreenRight() + d, y_slot[y] - zombie->GetSpriteHeight() / 2);
	zombie->move();

	return zombie;
}


//BucketheadZombie
Zombie* CGameMain::create_buc_zombie(int y) {
	float y_slot[5] = { -17, -5, 9, 20, 32 };
	BucketheadZombie* zombie = new BucketheadZombie(CSystem::MakeSpriteName(t_buc_zombie->GetName(), vec_buc_zombie.size()));
	vec_buc_zombie.push_back(zombie);
	name_to_sprite[zombie->GetName()] = zombie;
	zombie->CloneSprite(t_buc_zombie->GetName());
	zombie->set_exist(true);
	int d = CSystem::RandomRange(10, 20);
	zombie->SetSpritePosition(CSystem::GetScreenRight() + d, y_slot[y] - zombie->GetSpriteHeight() / 2);
	zombie->move();

	return zombie;
}

Zombie* CGameMain::create_new_zombie(int y) {
	float y_slot[5] = { -17, -5, 9, 20, 32 };
	NewspaperZombie* zombie = new NewspaperZombie(CSystem::MakeSpriteName(t_new_zombie->GetName(), vec_new_zombie.size()));
	vec_new_zombie.push_back(zombie);
	name_to_sprite[zombie->GetName()] = zombie;
	zombie->CloneSprite(t_new_zombie->GetName());
	zombie->set_exist(true);
	int d = CSystem::RandomRange(10, 20);
	zombie->SetSpritePosition(CSystem::GetScreenRight() + d, y_slot[y] - zombie->GetSpriteHeight() / 2);
	zombie->move();

	return zombie;
}

Zombie* CGameMain::create_fot_zombie(int y) {
	float y_slot[5] = { -17, -5, 9, 20, 32 };
	FootballZombie* zombie = new FootballZombie(CSystem::MakeSpriteName(t_fot_zombie->GetName(), vec_fot_zombie.size()));
	vec_fot_zombie.push_back(zombie);
	name_to_sprite[zombie->GetName()] = zombie;
	zombie->CloneSprite(t_fot_zombie->GetName());
	zombie->set_exist(true);
	int d = CSystem::RandomRange(10, 20);
	zombie->SetSpritePosition(CSystem::GetScreenRight() + d, y_slot[y] - zombie->GetSpriteHeight() / 2);
	zombie->move();

	return zombie;
}

Plant* CGameMain::create_pea_shooter(float x, float y) {
	// �����㶹���ֵ� ������Χ
	Range* rect = new Range(CSystem::MakeSpriteName(t_range->GetName(), vec_range.size()));
	vec_range.push_back(rect);
	name_to_sprite[rect->GetName()] = rect;
	rect->CloneSprite(t_range->GetName());
	rect->set_exist(true);

	// �����㶹���ֵ� �㶹
	Pea* pea = new Pea(CSystem::MakeSpriteName("Pea", vec_pea.size()));
	vec_pea.push_back(pea);
	name_to_sprite[pea->GetName()] = pea;
	pea->set_exist(false);

	// �����㶹����
	PeaShooter* pshtr = new PeaShooter(CSystem::MakeSpriteName(t_pea_shooter->GetName(), vec_pea_shooter.size()), pea);
	vec_pea_shooter.push_back(pshtr);
	name_to_sprite[pshtr->GetName()] = pshtr;

	pshtr->CloneSprite(t_pea_shooter->GetName());
	pshtr->SetSpritePosition(x, y);
	pshtr->SetSpriteImmovable(true);
	rect->SpriteMountToSprite(pshtr->GetName(), 11, -0.5);
	pshtr->set_exist(false);
	return pshtr;
}

Plant* CGameMain::create_sunflower(float x, float y, long double fTimeDelta) {
	Sun* sun = new Sun(CSystem::MakeSpriteName(t_sun->GetName(), vec_sun.size()), 25);
	vec_sun.push_back(sun);
	name_to_sprite[sun->GetName()] = sun;
	sun->set_exist(false);
	std::cout << sun->is_exist() << std::endl;

	Sunflower* sf = new Sunflower(CSystem::MakeSpriteName(t_sunflower->GetName(), vec_sunflower.size()), sun, fTimeDelta);
	vec_sunflower.push_back(sf);
	name_to_sprite[sf->GetName()] = sf;

	sf->CloneSprite(t_sunflower->GetName());
	sf->SetSpritePosition(x, y);
	sf->SetSpriteImmovable(true);
	sf->set_exist(false);
	return sf;
}

Plant* CGameMain::create_cherry_bomb(float x, float y, long double plant_time) {

	// ����ӣ��ը���� ��ը
	Boom* boom = new Boom(CSystem::MakeSpriteName("Boom", vec_boom.size()));
	vec_boom.push_back(boom);
	name_to_sprite[boom->GetName()] = boom;
	boom->set_exist(false);

	CherryBomb* cb = new CherryBomb(CSystem::MakeSpriteName(t_cherry_bomb->GetName(), vec_cherry_bomb.size()), boom, plant_time);
	std::cout << cb->get_type();
	vec_cherry_bomb.push_back(cb);
	name_to_sprite[cb->GetName()] = cb;

	cb->CloneSprite(t_cherry_bomb->GetName());
	cb->SetSpritePosition(x, y);
	cb->SetSpriteImmovable(true);
	//rect->SpriteMountToSprite(cb->GetName(), 0, 0);
	cb->set_exist(false);
	return cb;
}

Plant* CGameMain::create_wall_nut(float x, float y) {

	WallNut* wn = new WallNut(CSystem::MakeSpriteName(t_wall_nut->GetName(), vec_wall_nut.size()));
	vec_wall_nut.push_back(wn);
	name_to_sprite[wn->GetName()] = wn;
	wn->CloneSprite(t_wall_nut->GetName());
	wn->SetSpritePosition(x, y);
	wn->SetSpriteImmovable(true);
	wn->set_exist(false);
	return wn;
}

Plant* CGameMain::create_potato_mine(float x, float y, long double plant_time) {
	std::cout << "Potato Mine" << std::endl;
	// �����������׵� ������Χ
	Range* rect = new Range(CSystem::MakeSpriteName(t_range->GetName(), vec_range.size()));
	vec_range.push_back(rect);
	name_to_sprite[rect->GetName()] = rect;
	rect->CloneSprite(t_range->GetName());
	rect->set_exist(true);
	rect->SetSpriteWidth(16.375);
	rect->SetSpriteHeight(11.500);

	// �����������׵� ��ը
	Boom* boom = new Boom(CSystem::MakeSpriteName("Boom", vec_boom.size()));
	vec_boom.push_back(boom);
	name_to_sprite[boom->GetName()] = boom;
	boom->set_exist(false);

	PotatoMine* pm = new PotatoMine(CSystem::MakeSpriteName(t_potato_mine->GetName(), vec_potato_mine.size()), boom, plant_time);
	std::cout << pm->get_type();
	vec_potato_mine.push_back(pm);
	name_to_sprite[pm->GetName()] = pm;

	pm->CloneSprite(t_potato_mine->GetName());
	pm->SetSpritePosition(x, y);
	pm->SetSpriteImmovable(true);

	rect->SpriteMountToSprite(pm->GetName(), 0, 0);
	pm->set_exist(false);
	return pm;
}

Plant* CGameMain::create_jalapeno(float x, float y, long double plant_time) {
	// ������������ ��ը
	Boom* boom = new Boom(CSystem::MakeSpriteName("Boom", vec_boom.size()));
	vec_boom.push_back(boom);
	name_to_sprite[boom->GetName()] = boom;
	boom->set_exist(false);

	Jalapeno* jp = new Jalapeno(CSystem::MakeSpriteName(t_jalapeno->GetName(), vec_jalapeno.size()), boom, plant_time);
	vec_jalapeno.push_back(jp);
	name_to_sprite[jp->GetName()] = jp;

	jp->CloneSprite(t_jalapeno->GetName());
	jp->SetSpritePosition(x, y);
	jp->SetSpriteImmovable(true);
	jp->set_exist(false);
	return jp;
}

Plant* CGameMain::create_three_peater(float x, float y) {
	std::vector<Pea*> peas;
	for (int i = 0; i < 3; i++) {
		Pea* pea = new Pea(CSystem::MakeSpriteName(t_pea->GetName(), vec_pea.size()));
		vec_pea.push_back(pea);
		name_to_sprite[pea->GetName()] = pea;
		pea->set_exist(false);
		peas.push_back(pea);
	}

	ThreePeater* tp = new ThreePeater(CSystem::MakeSpriteName(t_three_peater->GetName(), vec_three_peater.size()), peas);
	vec_three_peater.push_back(tp);
	name_to_sprite[tp->GetName()] = tp;

	tp->CloneSprite(t_three_peater->GetName());
	tp->SetSpritePosition(x, y);
	tp->SetSpriteImmovable(true);
	tp->set_exist(false);

	for (int i = 0; i < 3; i++) {
		Range* r = new Range(CSystem::MakeSpriteName(t_range->GetName(), vec_range.size()));
		vec_range.push_back(r);
		name_to_sprite[r->GetName()] = r;
		r->CloneSprite(t_range->GetName());
		r->set_exist(true);
		if (i == 0) {
			r->SetSpriteRotation(15);
			r->SpriteMountToSprite(tp->GetName(), 10, 2.5);
		}
		else if (i == 2) {
			r->SetSpriteRotation(-15);
			r->SpriteMountToSprite(tp->GetName(), 10, -2.5);
		}
		else {
			r->SpriteMountToSprite(tp->GetName(), 11, 0);
		}
	}
	return tp;
}

void CGameMain::create_gray_mask(Card* card) {
	std::string name = std::string(card->GetName()) + "Mask";
	Assist* gray_mask;
	gray_mask = new Assist(name.c_str());
	name_to_sprite[name.c_str()] = gray_mask;
	gray_mask->DeleteSprite();
	gray_mask->CloneSprite("GrayMask");
	gray_mask->SetSpritePosition(card->GetSpritePositionX(), card->GetSpritePositionY());
	gray_mask->SpriteMountToSprite(card->GetName(), 0, 0);
	card->set_mask(gray_mask);
}

std::vector<PvZSprite*> CGameMain::get_sprites_by_position(float x, float y) {
	std::cout << "CLICK:" << x << " " << y << std::endl;
	std::vector<PvZSprite*> res;
	for (std::pair<std::string, PvZSprite*> sprite : name_to_sprite) {
		if (sprite.second->is_exist() && sprite.second->IsPointInSprite(x, y) && sprite.second->get_type() != "Range") {
			res.push_back(sprite.second);
		}
	}
	// ��ֵ���� �������Ч��
	return std::move(res);
}

bool CGameMain::planting(Plant* plant) {
	if (sun_count >= plant->get_cost()) {
		sun_count -= plant->get_cost();
		sun_num->SetTextValue(sun_count);
		return true;
	}
	return false;
}

void CGameMain::output_sun(int num) {
	Sun* sun = new Sun(CSystem::MakeSpriteName(t_sun->GetName(), vec_sun.size()), num);
	vec_sun.push_back(sun);
	name_to_sprite[sun->GetName()] = sun;
	sun->CloneSprite(t_sun->GetName());

	int pos_x = CSystem::RandomRange(CSystem::GetScreenLeft() + 5, CSystem::GetScreenRight() - 5);
	int pos_y = CSystem::RandomRange(CSystem::GetScreenTop() + 10, CSystem::GetScreenBottom() - 5);

	sun->SetSpritePosition(pos_x, CSystem::GetScreenTop() + 10);
	sun->SpriteMoveTo(pos_x, pos_y, 15, true);

	sun->set_exist(true);
	sun->SetSpriteLifeTime(10);
}

void CGameMain::add_sun(int num) {
	sun_count += num;
	sun_num->SetTextValue(sun_count);
}

void CGameMain::move_bowling_card() {
	for (Card* wmc : vec_card) {
		if (wmc->is_exist()) {
			wmc->SetSpriteImmovable(false);
			wmc->SetSpriteLinearVelocityX(-10);
		}
	}
}

void CGameMain::load_adventure_level(int level_id, long double fDeltaTime) {

	static int time_a_game = 180; // һ��ʱ��
	static int row_min = 0, row_max = 4; // ��ʬ���ɱ߽�
	const float first_zombie = 35;
	static float zombie_interval = first_zombie; // ��ʬ���ɱ߽�
	// ��ǰ��ͼΪ ð��ģʽ�ĵ�ͼ ��δ��ʼ��
	if (adventure_init == false) {
		// �������ļ����ؽ�ʬ������Ϣ
		char level[20];
		sprintf(level, "level_%d", level_id);
		std::cout << level << std::endl;

		if (level_id == 1) {
			row_min = 1, row_max = 3;
		}
		else {
			row_min = 0, row_max = 4;
		}

		// ��ð��ģʽconfig���ݹؿ�����ȡ�ؿ���Ϣ
		OrdinaryZombieCount		= GetPrivateProfileInt(level, "OrdinaryZombie", 0, "./adventureConfig.ini");
		BarricadeZombieCount	= GetPrivateProfileInt(level, "BarricadeZombie", 0, "./adventureConfig.ini");
		BucketheadZombieCount	= GetPrivateProfileInt(level, "BucketheadZombie", 0, "./adventureConfig.ini");
		NewspaperZombieCount	= GetPrivateProfileInt(level, "NewspaperZombie", 0, "./adventureConfig.ini");
		FootballZombieCount		= GetPrivateProfileInt(level, "FootballZombie", 0, "./adventureConfig.ini");

		total_zombie = OrdinaryZombieCount +
			BarricadeZombieCount +
			BucketheadZombieCount +
			NewspaperZombieCount +
			FootballZombieCount;
		time_a_game = total_zombie * 15;
		for (Card* card : vec_card) {
			create_gray_mask(card);
			card->plant_time(fDeltaTime);
		}

		// ����С��
		for (int i = row_min; i <= row_max; i++) {
			create_car(-47.5, -5 + -17 + i * 12)->set_exist(true);
		}

		sun_num->SetTextValue(sun_count);
		adventure_init = true;
		game_start = fDeltaTime;
		zombie_interval = first_zombie;
		zombie_timer = fDeltaTime;
	}

	if (adventure_init == true) {
		if (fDeltaTime - timer > 15) {
			if (level_id < 4) {
				output_sun();
				timer = fDeltaTime;
			}
		}

		if ((fDeltaTime - game_start) / time_a_game <= 1) {
			// ������Ϸʱ�� ���ý���������
			progress_bar->SetSpriteWidth(18 * (fDeltaTime - game_start) / time_a_game);
			progress_bar->SetSpritePosition(44.000 - progress_bar->GetSpriteWidth() / 2, 35.171);
			progress_head->SetSpritePosition(44.000 - progress_bar->GetSpriteWidth(), 35.171);
		}

		// �����Ⱦ��ʬ
		if (fDeltaTime - zombie_timer > zombie_interval) {
			if (zombie_interval == first_zombie) {
				SuperSound::closeAndPlay("open-begin", "play-begin", "close-begin", 4);
			}
			zombie_interval /= 1.5;
			if (zombie_interval < 5) {
				zombie_interval = 5;
			}
			zombie_timer = fDeltaTime;
			int zombie_type = CSystem::RandomRange(row_min, row_max);
			
			// break��if��н�ʬ������break����Ȼ�Ͳ�����һ�ֽ�ʬ����֤���н�ʬ������Խ����Ľ�ʬ�������ĸ���Խ��
			switch (zombie_type)
			{
			case 0:
				if (zombie_count > 2 && FootballZombieCount) {
					FootballZombieCount--;
					create_fot_zombie(CSystem::RandomRange(row_min, row_max));
					break;
				}
			case 1:
				if (zombie_count > 2 && NewspaperZombieCount) {
					NewspaperZombieCount--;
					create_new_zombie(CSystem::RandomRange(row_min, row_max));
					break;
				}
			case 2:
				if (zombie_count > 2 && BucketheadZombieCount) {
					BucketheadZombieCount--;
					create_buc_zombie(CSystem::RandomRange(row_min, row_max));
					break;
				}
			case 3:
				if (zombie_count > 2 && BarricadeZombieCount) {
					BarricadeZombieCount--;
					create_bar_zombie(CSystem::RandomRange(row_min, row_max));
					break;
				}
			case 4:
				if (OrdinaryZombieCount) {
					OrdinaryZombieCount--;
					create_ord_zombie(CSystem::RandomRange(row_min, row_max));
					break;
				}
			default:
				if (OrdinaryZombieCount) {
					OrdinaryZombieCount--;
					create_ord_zombie(CSystem::RandomRange(row_min, row_max));
					break;
				}
				if (zombie_count > 2 && FootballZombieCount) {
					FootballZombieCount--;
					create_fot_zombie(CSystem::RandomRange(row_min, row_max));
					break;
				}

				if (zombie_count > 2 && NewspaperZombieCount) {
					NewspaperZombieCount--;
					create_new_zombie(CSystem::RandomRange(row_min, row_max));
					break;
				}
				if (zombie_count > 2 && BucketheadZombieCount) {
					BucketheadZombieCount--;
					create_buc_zombie(CSystem::RandomRange(row_min, row_max));
					break;
				}
				if (zombie_count > 2 && BarricadeZombieCount) {
					BarricadeZombieCount--;
					create_bar_zombie(CSystem::RandomRange(row_min, row_max));
					break;
				}

				if (OrdinaryZombieCount) {
					OrdinaryZombieCount--;
					create_ord_zombie(CSystem::RandomRange(row_min, row_max));
					break;
				}
				break;
			}
			zombie_count++;
			zombie_timer = fDeltaTime;
		}
		// ���տ���������
		for (Sunflower* sunflower : vec_sunflower) {
			if (sunflower->is_exist()) {
				sunflower->attack(fDeltaTime);
			}
		}
		// ������ͷ
		for (PotatoMine* pm : vec_potato_mine) {
			if (pm->is_exist()) {
				pm->preparation(fDeltaTime);
			}
		}
		// ӣ�ұ�ը
		for (CherryBomb* cb : vec_cherry_bomb) {
			if (cb->is_exist()) {
				cb->preparation(fDeltaTime);
			}
		}
		// ������ը
		for (Jalapeno* jp : vec_jalapeno) {
			if (jp->is_exist()) {
				jp->preparation(fDeltaTime);
			}
		}
		// ����ȴ
		for (Card* card : vec_card) {
			card->ready(fDeltaTime);
		}
	}
}

void CGameMain::load_bowling(long double fDeltaTime) {
	static int zombie_counter = 0;
	static int time_a_game = 30; // һ��ʱ��
	const float first_zombie = 10;
	static float zombie_interval = first_zombie; // ��ʬ���ɱ߽�
	if (bowling_init == false) {
		CSprite convery_belt("ConveryBelt");
		convery_belt.SetSpriteImmovable(true);
		bowling_init = true;
		zombie_counter = 15;
		total_zombie = zombie_counter;
		bowling_counter = 0;
		time_a_game = zombie_counter * 7;
		game_start = fDeltaTime;
		zombie_interval = first_zombie;
	}
	if (bowling_init == true) {

		if ((fDeltaTime - game_start) / time_a_game <= 1) {
			// ������Ϸʱ�� ���ý���������
			std::cout << "sss" << std::endl;
			progress_bar->SetSpriteWidth(18 * (fDeltaTime - game_start) / time_a_game);
			progress_bar->SetSpritePosition(44.000 - progress_bar->GetSpriteWidth() / 2, 35.171);
			progress_head->SetSpritePosition(44.000 - progress_bar->GetSpriteWidth(), 35.171);
		}

		if (fDeltaTime - timer > 3) {
			if (zombie_counter > 0) {
				zombie_counter--;
				create_ord_zombie(CSystem::RandomRange(0, 4));
			}

			

			if (bowling_counter < 10) {

				WallNutCard* wnc = new WallNutCard(CSystem::MakeSpriteName(wall_nut_card->GetName(), vec_card.size()));
				vec_card.push_back(wnc);
				name_to_sprite[wnc->GetName()] = wnc;
				wnc->CloneSprite("WallNutCard");
				wnc->SetSpritePosition(14.310, -32.600);
				wnc->SetSpriteLinearVelocityX(-10);
				bowling_counter++;
			}
			timer = fDeltaTime;
		}
	}
}

void CGameMain::load_lianliankan(long double fDeltaTime)
{
	static int time_a_game = 120; // һ��ʱ��
	static int statr_timer = fDeltaTime;
	if (llk_init == false) {

		// ��ʼ������������
		LianLianKan::init();

		game_start = fDeltaTime;
		llk_init = true;
	}
	if ((fDeltaTime - game_start) / time_a_game <= 1) {
		// ������Ϸʱ�� ���ý���������
		progress_bar->SetSpriteWidth(18 * (fDeltaTime - game_start) / time_a_game);
		progress_bar->SetSpritePosition(44.000 - progress_bar->GetSpriteWidth() / 2, 35.171);
		progress_head->SetSpritePosition(44.000 - progress_bar->GetSpriteWidth(), 35.171);
	}
}

void CGameMain::reload() {
	CStaticSprite flag("Flag");
	flag.SetSpriteEnable(true);

	for (auto p : vec_sun) {
		p->DeleteSprite();
	}
	sun_count = 100;
	adventure_init = false;
	bowling_init = false;
	llk_init = false;
	zombie_count = 0;
}


void CGameMain::zombie_wave() {
	CStaticSprite flag("Flag");
	flag.SetSpriteEnable(false);
	std::cout << "wave" << std::endl;
	int row_min = 0, row_max = 4; // ��ʬ���ɱ߽�
	if (this->adventure_level_id == 1) {
		row_min = 1;
		row_max = 3;
	}

	for (int i = row_min; i <= row_max; i++) {
		if (CSystem::RandomRange(0, 1) && FootballZombieCount > 0 && adventure_level_id != 1) {
			create_fot_zombie(i);
			//total_zombie++;
			FootballZombieCount--;
		}

		if (CSystem::RandomRange(0, 1) && NewspaperZombieCount > 0 && adventure_level_id != 1) {
			create_new_zombie(i);
			//total_zombie++;
			NewspaperZombieCount--;
		}

		if (CSystem::RandomRange(0, 1) && BucketheadZombieCount > 0 && adventure_level_id != 1) {
			create_buc_zombie(i);
			//total_zombie++;
			BucketheadZombieCount--;
		}

		if (CSystem::RandomRange(0, 1) && OrdinaryZombieCount > 0) {
			create_ord_zombie(i);
			//total_zombie++;
			OrdinaryZombieCount--;
		}

		if (CSystem::RandomRange(0, 1) && BarricadeZombieCount > 0 && adventure_level_id != 1) {
			create_bar_zombie(i);
			//total_zombie++;
			BarricadeZombieCount--;
		}
	}
}