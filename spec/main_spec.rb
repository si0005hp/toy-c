describe 'main' do
  before do
  end

  def run_script(commands)
    raw_output = nil
    IO.popen("./main", "r+") do |pipe|
      commands.each do |command|
        pipe.puts command
      end

      pipe.close_write

      # Read entire output
      raw_output = pipe.gets(nil)
    end
    raw_output.split("\n")
  end

  def run_script_as_main_func_with_print(commands)
    arr = []
    commands.each do |command|
      IO.popen("./main", "r+") do |pipe|
        pipe.puts "int main() {"
        pipe.puts "print(" + command + ");"
        pipe.puts "}"
        pipe.close_write

        # Read entire output
        raw_output = pipe.gets(nil)
        arr.push(raw_output.chomp)
      end 
    end
    arr
  end

  def run_script_as_main_func(commands)
    raw_output = nil
    IO.popen("./main", "r+") do |pipe|
      pipe.puts "int main() {"
      commands.each do |command|
        pipe.puts command
      end
      pipe.puts "}"

      pipe.close_write

      # Read entire output
      raw_output = pipe.gets(nil)
    end
    raw_output.split("\n")
  end

  def run_script_with_file(fname)
    raw_output = nil
    IO.popen("./main %s" % File.expand_path(fname), "r+") do |pipe|
      pipe.close_write

      # Read entire output
      raw_output = pipe.gets(nil)
    end
    raw_output.split("\n")
  end

  it 'integer' do
    result = run_script_as_main_func_with_print([
      "1",
      "999",
    ])
    expect(result).to eq([
      "1",
      "999",
    ])
  end
  
  it 'add' do
    result = run_script_as_main_func_with_print([
      "1+2",
      "1 + 2",
      "1 + 2 + 3 + 4 + 5",
    ])
    expect(result).to eq([
      "3",
      "3",
      "15",
    ])
  end
  
  it 'sub' do
    result = run_script_as_main_func_with_print([
      "3-1",
      "10 - 3 - 2 - 1",
      "5 + 3 - 2 + 1 - 4 + 9",
    ])
    expect(result).to eq([
      "2",
      "4",
      "12",
    ])
  end
  
  it 'mul' do
    result = run_script_as_main_func_with_print([
      "2*5",
      "2 * 3 * 4",
      "1 * 2 + 3 + 4 * 5",
      "1 * 2 + 3 - 4 * 5",
    ])
    expect(result).to eq([
      "10",
      "24",
      "25",
      "-15",
    ])
  end
  
  it 'div' do
    result = run_script_as_main_func_with_print([
      "10/2",
      "99 / 3 / 11",
      "10 / 2 * 3 + 5 - 3 + 4 * 2 / 8",
    ])
    expect(result).to eq([
      "5",
      "3",
      "18",
    ])
  end
  
  it 'paren' do
    result = run_script_as_main_func_with_print([
      "(1)",
      "(((1)))",
      "(1 + 3)",
      "(2 * 3)",
      "(1 + 3) * 5",
      "(1 + 3) * 5 + (4 / 2) + 1",
      "(1 + 3) * 5 + (((4 + 2) / 3) * 3)",
      "(1 + 2) * (3 + 4)",
      "(3 * 4) / (1 + 2)",
    ])
    expect(result).to eq([
      "1",
      "1",
      "4",
      "6",
      "20",
      "23",
      "26",
      "21",
      "4",
    ])
  end
  
  it 'float' do
    result = run_script_as_main_func_with_print([
      "1.2",
      "1.2345",
      "1 + 2.345 + 6",
    ])
    expect(result).to eq([
      "1.2",
      "1.2345",
      "9.345",
    ])
  end

  it 'var' do
    result = run_script_as_main_func([
      "int i = 5;",
      "print(i);",
    ])
    expect(result).to eq(["5"])

    result = run_script_as_main_func([
      "int i = 1 + 3 * 4 / 2 + 90 - 10;",
      "print(i);",
    ])
    expect(result).to eq(["87"])

    result = run_script_as_main_func([
      "int x = 3;",
      "int y = 5;",
      "print(x);",
      "print(y);",
    ])
    expect(result).to eq([
      "3",
      "5",
    ])

    result = run_script_as_main_func([
      "int x = 3;",
      "x = 5;",
      "print(x);",
    ])
    expect(result).to eq([
      "5",
    ])

    result = run_script_as_main_func([
      "int x = 5;",
      "int y = x + 3;",
      "int z = x + y + 1;",
      "print(x);",
      "print(y);",
      "print(z);",
      "print(x + y + z);",
    ])
    expect(result).to eq([
      "5",
      "8",
      "14",
      "27",
    ])
  end

  it 'func' do 
    result = run_script_with_file("test/func/simple_func.c")
    expect(result).to eq(["1"])

    result = run_script_with_file("test/func/simple_func2.c")
    expect(result).to eq(["1"])

    result = run_script_with_file("test/func/simple_func3.c")
    expect(result).to eq(["1"])
  end
end
