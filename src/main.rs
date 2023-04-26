fn main() {
    let mut test = 5u64;

    test = if true {
        let test = test + 4;
        test
    } else {
        5
    };
    println!("Hello, world! + {test}");

    for x in 0..=5 {
        println!("{x}");
    }

    let mut age_du_capitaine: Vec<String> = Vec::new();

    age_du_capitaine.push(test.to_string());
    age_du_capitaine.push("test".to_string());

    for x in age_du_capitaine.iter() {
        println!("{x}");
    }

    println!("{:?}", age_du_capitaine);

    let oe = fonction("test", "zutalor");
}

fn fonction(prout: &str, p2: &str) -> Option<&'static u32> {
    Some(&32)
}
#[derive(Hash)]
struct St {
    e: String,
}

impl St {
    fn new() -> Self {
        Self {
            e: "oe".to_string(),
        }
    }
    fn membre(&mut self, genant: u128) {
        self.e = genant.to_string();
    }
}

fn test() {
    let mut oeoeoe: St = St::new();
    oeoeoe.membre(21);
}

trait Tr {
    fn oe();
    fn c();
    fn lon();
}
